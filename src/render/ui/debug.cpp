/*
 * Created by Brett on 15/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/ui/debug.h>
#include <render/ui/text.h>
#include <render/window.h>
#include <blt/math/averages.h>
#include "render/camera.h"

namespace fp::debug {
    const float spacing = 5;
    
    bool enabled = false;
    
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
        
        std::string fps = "FPS: ";
        fps += std::to_string(1.0 / fp::window::getFrameDelta());
        fps += " (";
        fps += std::to_string((double) fp::window::getFrameDeltaRaw() / 1000000.0);
        fps += "ms)";
        drawAndIncrement(fps, x_offset * 2, left_y_pos);
        
        auto& camera_pos = fp::camera::getPosition();
        std::string pos = "Position: ";
        pos += std::to_string(camera_pos.x());
        pos += " ";
        pos += std::to_string(camera_pos.y());
        pos += " ";
        pos += std::to_string(camera_pos.z());
    
        drawAndIncrement(pos, x_offset * 2, left_y_pos);
    }
}
