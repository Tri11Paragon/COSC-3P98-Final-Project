/*
 * Created by Brett on 14/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_TEXT_H
#define FINALPROJECT_TEXT_H

#include <blt/std/math.h>
#include <string>

namespace fp::text {
    
    struct text_size {
        int w,h;
    };
    
    enum font_size {
        FONT_11 = 11,
        FONT_12 = 12,
        FONT_14 = 14,
        FONT_18 = 18,
        FONT_22 = 22,
        FONT_36 = 36,
        FONT_48 = 48,
        FONT_72 = 72
    };
    
    void init();
    
    void render();
    
    void drawText(
            const std::string& text, float x, float y, font_size size, const blt::vec4& color = {1.0, 1.0, 1.0, 1.0},
            const blt::vec4& backgroundColor = {0.0, 0.0, 0.0, 0.0}, float scale = 1
    );
    
    text_size getTextSize(const std::string& text, font_size size, float scale = 1.0);
    
    void destroy();
}

#endif //FINALPROJECT_TEXT_H
