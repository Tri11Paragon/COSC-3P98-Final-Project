/*
 * Created by Brett on 16/03/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/renderer.h>

namespace fp {
    
    shader* renderer::createShader(shader&& shader) {
        auto s = new class shader(std::move(shader));
        shaders.push_back(s);
        return s;
    }
}