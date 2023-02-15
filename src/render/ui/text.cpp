/*
 * Created by Brett on 14/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/ui/text.h>
#include <blt/std/logging.h>
#include <render/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>

std::unordered_map<char, fp::text::gl_character> character_conversion_map;

FT_Library ft;
FT_Face monospaced_face;

void fp::text::init() {
    if (FT_Init_FreeType(&ft)) {
        BLT_FATAL("Unable to init freetype library!");
        std::abort();
    }
    if (FT_New_Face(ft, "assets/fonts/JetBrains Mono.ttf", 0, &monospaced_face)) {
        BLT_ERROR("Unable to load default monospaced (JetBrains Mono) font!");
        std::abort();
    }
    FT_Set_Pixel_Sizes(monospaced_face, 0, 48);
    
    // disable alignment restrictions. This might cause issues with WebGL! FIXME: if it does
    // gl requires an alignment of 4. Since we are going to only use a single character of any width/height the alignment must be changed.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // TODO: merge into a 2d texture array
    unsigned int textures[128];
    glGenTextures(128, textures);
    
    // we only care about ascii characters [0, 128). I won't be rendering with anything other than them.
    for (int i = 0; i < 128; i++){
        // tell freetype to render the character to a monochrome texture
        FT_Error error;
        if ((error = FT_Load_Char(monospaced_face, i, FT_LOAD_RENDER))) {
            BLT_ERROR("Unable to load character '%c' using the default monospaced font! (Error: %d)", i, error);
            glDeleteTextures(1, &textures[i]);
            continue;
        }
        auto glyph = monospaced_face->glyph;
        auto bitmap = glyph->bitmap;
        
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        // since the rendered texture is monochrome, there is no reason to allocate memory for more than one channel.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)bitmap.width, (int)bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
        // set standard texture options.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // linear because we want nice smoothing on our font
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        character_conversion_map.insert({i, gl_character{
            textures[i],
            {(int)bitmap.width, (int)bitmap.rows},
            {glyph->bitmap_left, glyph->bitmap_top},
            (unsigned int) glyph->advance.x
        }});
    }
    
    FT_Done_Face(monospaced_face);
    FT_Done_FreeType(ft);
}

void fp::text::destroy() {
    for (const auto& c : character_conversion_map)
        glDeleteTextures(1, &c.second.textureID);
}
