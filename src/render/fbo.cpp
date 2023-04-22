/*
 * Created by Brett on 16/03/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/fbo.h>

namespace fp {

    void FBO::attach(fp::texture::gl_texture* texture) {
        texture->bind();
        
    }
    
}