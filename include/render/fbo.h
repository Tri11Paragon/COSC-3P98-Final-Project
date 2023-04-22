/*
 * Created by Brett on 16/03/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_FBO_H
#define FINALPROJECT_FBO_H

#include <render/textures.h>

namespace fp {
    
    class FBO {
        private:
            unsigned int fboID = 0;
            bool m_screen_space = false;
        public:
            explicit FBO(bool screen_space = false): m_screen_space(screen_space) {
                glGenFramebuffers(1, &fboID);
            }
            
            void bind() const {
                glBindFramebuffer(GL_FRAMEBUFFER, fboID);
            }
            
            static void unbind() {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            
            void attach(fp::texture::gl_texture* texture);
            
            ~FBO() {
                glDeleteFramebuffers(1, &fboID);
            }
    };
    
}

#endif //FINALPROJECT_FBO_H
