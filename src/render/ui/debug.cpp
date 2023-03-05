/*
 * Created by Brett on 15/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/ui/debug.h>
#include <render/ui/text.h>
#include <render/window.h>
#include <blt/math/averages.h>

namespace fp::debug {
    const float spacing = 5;
    
    bool enabled = false;
    
    // Yes. I made this only for this.
    blt::averagizer_o_matic<double, 16> fps_average{60.0};
    
    void drawAndIncrement(const std::string& text, float x, float& pos, text::font_size size = fp::text::FONT_14) {
        auto text_size = fp::text::getTextSize(text, size);
        fp::text::drawText(text, x, pos, size, {0.0, 0.0, 0.0, 1.0});
        pos += spacing + (float) text_size.h;
    }
    
    void enable() {
        enabled = true;
    }
    
    void disable() {
        enabled = false;
    }
    
    void toggle() {
        enabled = !enabled;
    }
    
    void render() {
        if (fp::window::isKeyPressed(GLFW_KEY_F3) && fp::window::keyState())
            toggle();
        if (!enabled)
            return;
        
        float left_y_pos = 10;
        float right_y_pos = 10;
        
        float x_offset = 10;
        
        drawAndIncrement("Untitled Application", x_offset, left_y_pos, fp::text::FONT_22);
        fps_average.insert(1.0 / fp::window::getFrameDelta());
        
        std::string fps = "FPS: ";
        fps += std::to_string(fps_average.average());
        fps += " (";
        fps += std::to_string((double) fp::window::getFrameDeltaRaw() / 1000000.0);
        fps += "ms)";
        drawAndIncrement(fps, x_offset * 2, left_y_pos);
    }
}
