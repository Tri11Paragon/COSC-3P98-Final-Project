/*
 * Created by Brett on 17/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_TEXTURES_H
#define FINALPROJECT_TEXTURES_H

#include <string>
#include <utility>
#include <stb/stb_image.h>
#include "stb/stb_image_resize.h"

namespace fp::texture {
    
    class texture {
        private:
            std::string m_Name;
            std::string m_Path;
            int width = 0, height = 0, channels = 0;
            unsigned char* m_Data = nullptr;
        public:
            texture(std::string name, std::string path): m_Name(std::move(name)), m_Path(std::move(path)) {}
            
            static texture* load(texture* texture) {
                stbi_load(texture->m_Path.c_str(), &texture->width, &texture->height, &texture->channels, 4);
                return texture;
            }
            
            static texture* resize(texture* texture, int target_width, int target_height) {
                // since stb is a c lib we must use malloc, otherwise we'd have to deal with using the right free on destruction
                auto* output_Data = (unsigned char*) malloc(target_width * target_height * texture->channels);
                
                stbir_resize_uint8(
                        texture->m_Data, texture->width, texture->height, texture->width * texture->channels, output_Data, target_width,
                        target_height, target_width * texture->channels, texture->channels
                );

                stbi_image_free(texture->m_Data);
                texture->m_Data = output_Data;
                texture->width = target_width;
                texture->height = target_height;
            }
            
            unsigned char* data() {
                return m_Data;
            }
            
            ~texture() {
                stbi_image_free(m_Data);
            }
    };
    
    class palette {
        private:
            // as of GL3.0 this limit is 256. (4.5 extends it to 2048.)
            static constexpr int MAX_ARRAY_LAYERS = 256;
            
            int m_CurrentTextureCount = 0;
            unsigned int textureID = 0;
        
        public:
            palette() {
            
            }
    };
    
    
}


#endif //FINALPROJECT_TEXTURES_H
