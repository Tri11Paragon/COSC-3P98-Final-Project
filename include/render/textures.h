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
#include <util/settings.h>
#include <phmap.h>
#include <vector>
#include <render/gl.h>
#include "stb/stb_image_resize.h"


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
            explicit file_texture(const std::string& path, const std::string& name = ""):
                    m_Name(name.empty() ? path : name), m_Path(path) {}
            
            static file_texture* load(file_texture*& texture) {
                // we want to load every texture as if it has transparency,
                // otherwise textures won't be correctly resized and loaded to the gpu
                constexpr int channel_count = 4;
                texture->m_Data = stbi_load(
                        texture->m_Path.c_str(), &texture->width, &texture->height,
                        &texture->channels, channel_count
                );
                texture->channels = channel_count;
                return texture;
            }
            
            static file_texture* resize(
                    file_texture* texture, int target_width, int target_height
            ) {
                if (target_width == texture->width && target_height == texture->height)
                    return texture;
                // since we will be replacing the loaded data pointer, is it wise to use the allocator
                // that matches with what stb image uses, which is malloc, since we unload with stbi_image_free -> (free)
                auto* output_Data = (unsigned char*) malloc(
                        target_width * target_height * texture->channels
                );
                
                // resize the texture
                if (stbir_resize_uint8(
                        // input
                        texture->m_Data, texture->width, texture->height, 0,
                        // output
                        output_Data, target_width, target_height, 0,
                        // channels
                        texture->channels
                )) {
                    BLT_WARN("Error resizing block texture image!");
                }
                
                // free up the old data
                stbi_image_free(texture->m_Data);
                texture->m_Data = output_Data;
                texture->width = target_width;
                texture->height = target_height;
                
                return texture;
            }
            
            unsigned char* data() {
                return m_Data;
            }
            
            [[nodiscard]] int getChannels() const {
                return channels;
            }
            
            [[nodiscard]] int getWidth() const {
                return width;
            }
            
            [[nodiscard]] int getHeight() const {
                return height;
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
            
            gl_texture(
                    int width, int height, GLint bind_type = GL_TEXTURE_2D,
                    GLint color_mode = GL_RGBA
            ):
                    m_width(width), m_height(height), textureBindType(bind_type),
                    textureColorMode(color_mode) {
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
                glTexParameteri(textureBindType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                glTexParameteri(textureBindType, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                // Anisotropy helps preserve textures at oblique angles
                float a = 0;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &a);
                glTexParameterf(textureBindType, GL_TEXTURE_MAX_ANISOTROPY_EXT, a);
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
            
            virtual ~gl_texture() {
                glDeleteTextures(1, &textureID);
            }
    };
    
    struct gl_texture2D : public gl_texture {
        public:
            gl_texture2D(int width, int height, GLint colorMode = GL_RGBA):
                    gl_texture(width, height, GL_TEXTURE_2D, colorMode) {
                bind();
                glTexStorage2D(
                        textureBindType, std::stoi(fp::settings::get("MIPMAP_LEVELS")), colorMode,
                        width, height
                );
            }
            
            void upload(
                    void* data, GLint dataColorMode = GL_RGBA, int level = 0, int x_offset = 0,
                    int y_offset = 0, int sub_width = -1,
                    int sub_height = -1
            ) const {
                if (sub_width < 0)
                    sub_width = m_width;
                if (sub_height < 0)
                    sub_height = m_height;
                bind();
                glTexSubImage2D(
                        textureBindType, level, x_offset, y_offset, sub_width, sub_height,
                        dataColorMode, GL_UNSIGNED_BYTE, data
                );
                unbind();
            }
            
            void upload(file_texture* texture) const {
                upload(texture->data(), texture->getChannels() == 4 ? GL_RGBA : GL_RGB);
            }
    };
    
    struct gl_texture2D_array : public gl_texture {
        protected:
            int m_layers;
        public:
            gl_texture2D_array(int width, int height, int layers, GLint colorMode = GL_RGBA8):
                    gl_texture(width, height, GL_TEXTURE_2D_ARRAY, colorMode), m_layers(layers) {
                bind();
                // 6+ mipmaps is about where I stop noticing any difference (size is 4x4 pixels, so that makes sense)
                glTexStorage3D(textureBindType, 6, colorMode, width, height, layers);
                BLT_DEBUG("Creating 2D Texture Array with ID: %d", textureID);
            }
            
            void upload(
                    void* data, int index, GLint dataColorMode = GL_RGBA, int level = 0,
                    int x_offset = 0, int y_offset = 0, int sub_width = -1,
                    int sub_height = -1
            ) const {
                if (sub_width < 0)
                    sub_width = m_width;
                if (sub_height < 0)
                    sub_height = m_height;
                bind();
                glTexSubImage3D(
                        textureBindType, level, x_offset, y_offset, index, sub_width, sub_height, 1,
                        dataColorMode, GL_UNSIGNED_BYTE, data
                );
                unbind();
            }
    };
    
    typedef int texture_index;
    
    class palette {
        private:
            union negDInt {
                texture_index i = -1;
            };
            // as of GL3.0 this limit is 256. (4.5 extends it to 2048.)
            static constexpr int MAX_ARRAY_LAYERS = 256;
            
            gl_texture2D_array* texture_array = nullptr;
        
            phmap::flat_hash_map<std::string, negDInt> textureIndices;
            std::vector<file_texture*> textures;
        
        public:
            palette() = default;
            
            void generateGLTexture() {
                auto texture_size = std::stoi(fp::settings::get("TEXTURE_SIZE"));
                texture_array = new gl_texture2D_array(
                        texture_size, texture_size, (int) textures.size());
                texture_array->bind();
                for (const auto t : textures) {
                    texture_array->upload(
                            t->data(), textureIndices[t->getName()].i,
                            t->getChannels() == 4 ? GL_RGBA : GL_RGB
                    );
                    BLT_TRACE("Loaded texture %s", t->getName().c_str());
                }
                texture_array->setDefaults();
                texture_array->generateMipmaps();
            }
            
            void registerTexture(file_texture* texture) {
                textureIndices[texture->getName()].i = (int) textures.size();
                textures.push_back(texture);
            }
            
            bool hasTexture(const std::string& name) {
                return textureIndices[name].i >= 0;
            }
            
            texture_index getTexture(const std::string& name) {
                return textureIndices[name].i;
            }
            
            unsigned int getTextureID() {
                return texture_array->getTextureID();
            }
            
            ~palette() {
                for (auto* t : textures)
                    delete t;
                delete texture_array;
            };
    };
    
    
}


#endif //FINALPROJECT_TEXTURES_H
