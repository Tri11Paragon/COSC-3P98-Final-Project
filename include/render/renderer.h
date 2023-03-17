/*
 * Created by Brett on 16/03/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_RENDERER_H
#define FINALPROJECT_RENDERER_H

#include <render/gl.h>
#include <vector>

namespace fp {

    /**
     * Shaders stored within are guaranteed to exist as long as this instance exists
     */
    class renderer {
        private:
            std::vector<shader*> shaders;
        public:
            renderer() = default;
            
            shader* createShader(shader&& shader);
            
            ~renderer(){
                for (shader* shader : shaders)
                    delete shader;
            }
    };
    
}

#endif //FINALPROJECT_RENDERER_H
