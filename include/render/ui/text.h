/*
 * Created by Brett on 14/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_TEXT_H
#define FINALPROJECT_TEXT_H

namespace fp::text {
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
    
    void init();
    void destroy();
}

#endif //FINALPROJECT_TEXT_H
