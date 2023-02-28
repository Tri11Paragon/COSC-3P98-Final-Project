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
#include <util/settings.h>
#include <unordered_map>
#include <vector>
#include <render/gl.h>

namespace fp::texture {
    
    class file_texture {
        private:
            std::string m_Name;
            std::string m_Path;
            int width = 0, height = 0, channels = 0;
            unsigned char* m_Data = nullptr;
        public:
            /**
             * @param path path to the texture file
             * @param name reference name for this texture. If empty the texture will use path as its identifier
             */
            explicit file_texture(const std::string& path, const std::string& name = ""): m_Name(name.empty() ? path : name), m_Path(path) {}
            
            static file_texture* load(file_texture* texture) {
                stbi_load(texture->m_Path.c_str(), &texture->width, &texture->height, &texture->channels, 4);
                return texture;
            }
            
            static file_texture* resize(file_texture* texture, int target_width, int target_height) {
                if (target_width == texture->width && target_height == texture->height)
                    return texture;
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
            
            [[nodiscard]] const std::string& getName() {
                return m_Name;
            }
            
            ~file_texture() {
                stbi_image_free(m_Data);
            }
    };
    
    struct gl_texture {
        protected:
            unsigned int textureID = 0;
            GLint textureBindType;
            GLint textureColorMode;
            int m_width, m_height;
            
            gl_texture(int width, int height, GLint bind_type = GL_TEXTURE_2D, GLint color_mode = GL_RGBA):
                    m_width(width), m_height(height), textureBindType(bind_type), textureColorMode(color_mode) {
                glGenTextures(1, &textureID);
            }
        public:
            void bind() const {
                glBindTexture(textureBindType, textureID);
            }
            
            void unbind() const {
                glBindTexture(textureBindType, 0);
            }
            
            void setDefaults() const {
                bind();
                glTexParameteri(textureBindType, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(textureBindType, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // nearest preserves the pixely look
                glTexParameteri(textureBindType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(textureBindType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                unbind();
            }
            
            void generateMipmaps() const {
                // it's a little inefficient binding and unbinding for these small calls, they really should be done in the constructor or data upload
                bind();
                glGenerateMipmap(textureBindType);
                unbind();
            }
            
            [[nodiscard]] unsigned int getTextureID() const {
                return textureID;
            }
            
            ~gl_texture() {
                glDeleteTextures(1, &textureID);
            }
    };
    
    struct gl_texture2D : public gl_texture {
        public:
            gl_texture2D(int width, int height, GLint colorMode = GL_RGBA): gl_texture(width, height, GL_TEXTURE_2D, colorMode) {
                bind();
                glTexStorage2D(textureBindType, std::stoi(fp::settings::get("MIPMAP_LEVELS")), colorMode, width, height);
            }
            
            void upload(void* data, int level = 0, int x_offset = 0, int y_offset = 0, int sub_width = -1, int sub_height = -1) const {
                if (sub_width < 0)
                    sub_width = m_width;
                if (sub_height < 0)
                    sub_height = m_height;
                glTexSubImage2D(textureBindType, level, x_offset, y_offset, sub_width, sub_height, textureColorMode, GL_UNSIGNED_BYTE, data);
            }
    };
    
    struct gl_texture2D_array : public gl_texture {
        protected:
            int m_layers;
        public:
            gl_texture2D_array(int width, int height, int layers, GLint colorMode = GL_RGBA):
                    gl_texture(width, height, GL_TEXTURE_2D_ARRAY, colorMode), m_layers(layers) {
                bind();
                glTexStorage3D(textureBindType, std::stoi(fp::settings::get("MIPMAP_LEVELS")), colorMode, width, height, layers);
            }
        
            void upload(void* data, int index, int level = 0, int x_offset = 0, int y_offset = 0, int sub_width = -1, int sub_height = -1) const {
                if (sub_width < 0)
                    sub_width = m_width;
                if (sub_height < 0)
                    sub_height = m_height;
                glTexSubImage3D(textureBindType, level, x_offset, y_offset, index, sub_width, sub_height, 1, textureColorMode, GL_UNSIGNED_BYTE, data);
            }
    };
    
    class palette {
        private:
            union negDInt {
                int i = -1;
            };
            // as of GL3.0 this limit is 256. (4.5 extends it to 2048.)
            static constexpr int MAX_ARRAY_LAYERS = 256;
        
            gl_texture2D_array* texture_array = nullptr;
            
            std::unordered_map<std::string, negDInt> textureIndices;
            std::vector<file_texture*> textures;
        
        public:
            palette() = default;
            
            void generateGLTexture() {
                delete texture_array;
                auto texture_size = std::stoi(fp::settings::get("TEXTURE_SIZE"));
                texture_array = new gl_texture2D_array(texture_size, texture_size, (int)textures.size());
                for (const auto t : textures)
                    texture_array->upload(t->data(), textureIndices[t->getName()].i);
                texture_array->setDefaults();
                texture_array->generateMipmaps();
            }
            
            void registerTexture(file_texture* texture) {
                auto texture_size = std::stoi(fp::settings::get("TEXTURE_SIZE"));
                texture = file_texture::resize(texture, texture_size, texture_size);
                
                textureIndices[texture->getName()].i = (int) textures.size();
                textures.push_back(texture);
            }
            
            bool hasTexture(const std::string& name) {
                return textureIndices[name].i >= 0;
            }
            
            int getTexture(const std::string& name) {
                return textureIndices[name].i;
            }
            
            unsigned int getTextureID() {
                return texture_array->getTextureID();
            }
            
            ~palette() {
                delete texture_array;
            };
    };
    
    
}


#endif //FINALPROJECT_TEXTURES_H
