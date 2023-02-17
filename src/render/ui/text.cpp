/*
 * Created by Brett on 14/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/ui/text.h>
#include <blt/std/logging.h>
#include <queue>
#include <render/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <shaders/text.vert>
#include <shaders/text.frag>
#include <cmath>

struct int_vec2 {
    int x, y;
};

// https://freetype.org/freetype2/docs/glyphs/glyph-metrics-3.svg
struct gl_character {
    unsigned int textureID; // OpenGL texture ID that holds the character
    int_vec2 size; // size of the character glyph
    int_vec2 bearing; // offset to the top left of the glyph from the current cursor pos
    unsigned int advance; // distance to the next glyph
};

struct text_render_object {
    std::string text;
    float x = 0;
    float y = 0;
    fp::text::font_size size;
    blt::vec4 color;
    // currently does nothing
    blt::vec4 backgroundColor;
    // DO NOT USE SCALE UNLESS NECESSARY. Will distort font!
    float scale = 0;
    
    text_render_object() = default;
    
    text_render_object(
            std::string text, float x, float y, fp::text::font_size size, const blt::vec4& color,
            const blt::vec4& backgroundColor, float scale = 1.0
    ):
            text(std::move(text)), x(x), y(y), color(color), backgroundColor(backgroundColor), scale(scale), size(size) {
    }
};

std::unordered_map<fp::text::font_size, std::unordered_map<char, gl_character>> character_conversion_map;
std::queue<text_render_object> text_render_queue;

FT_Library ft;
FT_Face monospaced_face;

fp::shader* text_shader;
fp::VAO* quad_vao;

void generateCharacters(fp::text::font_size size) {
    
    FT_Set_Pixel_Sizes(monospaced_face, 0, size);
    
    // TODO: merge into a 2d texture array
    unsigned int textures[128];
    glGenTextures(128, textures);
    
    // we only care about ascii characters [0, 128). I won't be rendering with anything other than them.
    for (int i = 0; i < 128; i++) {
        // tell freetype to render the character to a monochrome texture
        FT_Error error;
        if ((error = FT_Load_Char(monospaced_face, i, FT_LOAD_RENDER))) {
            BLT_WARN("Unable to load character '%c' using the default monospaced font! (Error: %d)", i, error);
            glDeleteTextures(1, &textures[i]);
            continue;
        }
        auto glyph = monospaced_face->glyph;
        auto bitmap = glyph->bitmap;
        
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        // GL_RED isn't valid for webgl
#ifdef __EMSCRIPTEN__
        constexpr GLint internal_format = GL_R8;
#else
        constexpr GLint internal_format = GL_RED;
#endif
        // since the rendered texture is monochrome, there is no reason to allocate memory for more than one channel.
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, (int) bitmap.width, (int) bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
        // set standard texture options. GL_CLAMP_TO_BORDER would be preferable but isn't in standard webgl
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // linear because we want nice smoothing if our font is resized
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        character_conversion_map[size].insert(
                {(char) i, gl_character{
                        textures[i],
                        {(int) bitmap.width, (int) bitmap.rows},
                        {glyph->bitmap_left, glyph->bitmap_top},
                        (unsigned int) glyph->advance.x
                }}
        );
    }
}

void fp::text::init() {
    if (FT_Init_FreeType(&ft)) {
        BLT_FATAL("Unable to init freetype library!");
        std::abort();
    }
    if (FT_New_Face(ft, "assets/fonts/JetBrains Mono.ttf", 0, &monospaced_face)) {
        BLT_ERROR("Unable to load default monospaced (JetBrains Mono) font!");
        std::abort();
    }
    
    // disable alignment restrictions. This might cause issues with WebGL! FIXME: if it does
    // gl requires an alignment of 4. Since we are going to only use a single character of any width/height the alignment must be changed.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    generateCharacters(FONT_11);
    generateCharacters(FONT_12);
    generateCharacters(FONT_14);
    generateCharacters(FONT_18);
    generateCharacters(FONT_22);
    generateCharacters(FONT_36);
    generateCharacters(FONT_48);
    generateCharacters(FONT_72);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    FT_Done_Face(monospaced_face);
    FT_Done_FreeType(ft);
    
    // create the GL objects required to render texts
    text_shader = new shader(shader_text_vert, shader_text_frag);
    quad_vao = new VAO();
    
    float vertices[6 * 4] = {
            // vertices     uvs
            // triangle 1
            1.0, 0, 1.0f, 0.0f,
            0, 0, 0.0f, 0.0f,
            0, 1.0, 0.0f, 1.0f,
            // triangle 2
            1.0, 1.0, 1.0f, 1.0f,
            1.0, 0, 1.0f, 0.0f,
            0, 1.0, 0.0f, 1.0f,
    };
    
    
    quad_vao->bindVBO(new VBO(ARRAY_BUFFER, vertices, sizeof(float) * 6 * 4), 0, 4);
}

void fp::text::destroy() {
    for (const auto& c : character_conversion_map)
        for (const auto& t : c.second)
            glDeleteTextures(1, &t.second.textureID);
    delete (text_shader);
    delete (quad_vao);
}

void fp::text::render() {
    glEnable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    text_shader->use();
    quad_vao->bind();
    glEnableVertexAttribArray(0);
    
    while (!text_render_queue.empty()) {
        const auto& text_object = text_render_queue.front();
        
        text_shader->setVec4("text_color", text_object.color);
        glActiveTexture(GL_TEXTURE0);
        
        // will be incremented by advance
        float cursor_x = text_object.x;
        // we need to offset the y coord freetype / opengl is expecting to draw from the bottom left, but because of how I have the ortho matrix
        // the renderer draws from the top left (which I think is a little more intuitive). The easiest way to fix this is the offset.
        int max_height = 0;
        
        // but the offset must be the max height of the characters in the text otherwise the position of smaller chars like 'o' and 'e' will be off
        for (const char c : text_object.text) {
            const gl_character gl_char = character_conversion_map[text_object.size][c];
            max_height = std::max(gl_char.size.y, max_height);
        }
        
        // TODO: use instancing to reduce number of draw calls (use signed distance fields when you make the switch)
        // valve paper on the subject (signed distance fields):
        // https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
        for (const char c : text_object.text) {
            const gl_character gl_char = character_conversion_map[text_object.size][c];
            
            blt::mat4x4 translation_matrix{};
            
            auto w = (float) gl_char.size.x * text_object.scale;
            auto h = (float) gl_char.size.y * text_object.scale;
            
            // move the glyph based on https://freetype.org/freetype2/docs/glyphs/glyph-metrics-3.svg
            auto x = cursor_x + (float) gl_char.bearing.x * text_object.scale;
            auto y = (float) max_height + text_object.y + (float) (-gl_char.bearing.y) * text_object.scale;
            
            translation_matrix.translate(x, y, 0);
            translation_matrix.scale(w, h, 0);
            
            text_shader->setMatrix("translation", translation_matrix);
            
            glBindTexture(GL_TEXTURE_2D, gl_char.textureID);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // advance is loaded in as 1/64th pixels
            cursor_x += (float) (gl_char.advance / 64.0) * text_object.scale;
        }
        
        text_render_queue.pop();
    }
    
    glDisableVertexAttribArray(0);
    
    glDisable(GL_BLEND);
}

void fp::text::drawText(const std::string& text, float x, float y, fp::text::font_size size, const blt::vec4& color, const blt::vec4& backgroundColor, float scale) {
    text_render_queue.emplace(text, x, y, size, color, backgroundColor, scale);
}

fp::text::text_size fp::text::getTextSize(const std::string& text, fp::text::font_size size, float scale) {
    int w = 0, h = 0;
    for (const char& c : text) {
        const gl_character gl_char = character_conversion_map[size][c];
        
        auto local_w = (int)((float)gl_char.size.x * scale);
        auto local_h = (int)((float)gl_char.size.y * scale);
        
        w = std::max(w, local_w);
        h = std::max(h, local_h);
    }
    return {w, h};
}
