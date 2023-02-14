/*
 * Created by Brett on 08/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */

#ifndef FINALPROJECT_GL_H
#define FINALPROJECT_GL_H

// pulls in gl headers along with window tools
#include <render/window.h>
#include <vector>
#include <unordered_map>
#include <blt/std/math.h>
#include <blt/std/string.h>
#include <string>

namespace fp {
/**
 * Supported VBO types.
 */
    enum vbo_type {
        ARRAY_BUFFER = GL_ARRAY_BUFFER,
        ELEMENT_BUFFER = GL_ELEMENT_ARRAY_BUFFER
    };
    
    enum vbo_mem_type {
        STATIC = GL_STATIC_DRAW,
        STREAM = GL_STREAM_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW
    };
    
    struct VBO {
        GLuint vboID = 0;
        void* data = nullptr;
        int size = 0;
        vbo_type type = ARRAY_BUFFER;
        vbo_mem_type mem_type = STATIC;
        
        VBO(vbo_type type, void* data, int size, vbo_mem_type mem_type = STATIC): type(type), data(data), size(size), mem_type(mem_type) {
            glGenBuffers(1, &vboID);
            bind();
            glBufferData(type, size, data, mem_type);
        }
        
        template<typename T>
        VBO(vbo_type type, std::vector<T>& data, vbo_mem_type mem_type = STATIC): type(type), mem_type(mem_type) {
            glGenBuffers(1, &vboID);
            bind();
            size = data.size() * sizeof(float);
            glBufferData(type, size, data.data(), mem_type);
            //glBufferSubData(type, 0, )
        }
        
        inline void update(void* new_data, int data_size) {
            bind();
            // optimization technique is to not reallocate the memory on the GPU if the new buffer size is not larger than our current buffer
            if (data_size <= size){
                // we can do this as long as we overwrite from the beginning. Since the new draw call will only use of to size of the allocated buffer
                // to do all its drawing, the extra space unused can be ignored and saved for future use.
                glBufferSubData(type, 0, data_size, new_data);
            } else
                glBufferData(type, data_size, new_data, mem_type);
            data = new_data;
            size = data_size;
            glBindBuffer(type, 0);
        }
        
        template<typename T>
        inline void update(std::vector<T>& new_data) {
            update(new_data.data(), new_data.size() * sizeof(T));
        }
        
        inline void bind() const {
            glBindBuffer(type, vboID);
        }
        
        ~VBO() {
            glDeleteBuffers(1, &vboID);
        }
    };
    
    class VAO {
        private:
            GLuint vaoID = 0;
            // -1 will contain indices VBO
            std::unordered_map<int, VBO*> VBOs;
        public:
            VAO();
            
            // we should not be copying VAOs! VAOs should be allocated on the heap!
            VAO(const VAO& copy) = delete;
            
            VAO(VAO&& move) = delete;
            
            /**
             * binds the VBO as an attribute buffer for use in a shader
             * @param vbo vbo to bind
             * @param attribute_number attribute position to bind
             * @param coordinate_size size of this attribute (1 for float, 2 for vec2...)
             * @param type type to store. Use GL_FLOAT mostly
             * @param stride stride will automatically be calculated using coordinate size (4 bytes is assumed!)
             * @param offset offset into the data that this attribute is stored. Allows for weaving of data
             */
            void bindVBO(VBO* vbo, int attribute_number, int coordinate_size, GLenum type = GL_FLOAT, int stride = 0, long offset = 0);
            
            /**
             * Binds the VBO as if it was the element buffer (indices). Note: calling this more than once is not supported.
             * @param vbo vbo to use
             */
            void bindElementVBO(VBO* vbo);
            
            inline VBO* getVBO(int attribute_number) {
                return VBOs[attribute_number];
            }
            
            inline void bind() const {
                glBindVertexArray(vaoID);
            }
            
            ~VAO();
    };
    
    class shader {
        private:
            struct IntDefaultedToMinusOne {
                GLint i = -1;
            };
            // we can have shaders of many types in OpenGL
            unsigned int programID = 0;
            // but we will only make use of these two for now
            unsigned int vertexShaderID = 0;
            unsigned int fragmentShaderID = 0;
            // while these will remain unused. (Webgl2 apparently doesn't support them despite being based on GL4.3? that's a TODO!)
            unsigned int geometryShaderID = 0;
            // this would be very useful however it is highly unlikely webgl will support it
            // im leaving some of this stuff in here because I might expand the native application to use some of it.
            // im trying to keep the web and native versions the same though
            unsigned int tessellationShaderID = 0;
            std::unordered_map<std::string, IntDefaultedToMinusOne> uniformVars;
        
            static unsigned int createShader(const std::string& source, int type);
            
            inline GLint getUniformLocation(const std::string &name) {
                if (uniformVars[name].i != -1)
                    return uniformVars[name].i;
                // caching the result is a lot faster since it won't change after the shader is created.
                // TODO: look into this: https://webglfundamentals.org/webgl/lessons/webgl-qna-how-can-i-get-all-the-uniforms-and-uniformblocks.html
                int loc = glGetUniformLocation(programID, name.c_str());
                uniformVars[name].i = loc;
                return loc;
            }
            
            static inline std::string removeEmptyFirstLines(const std::string& string){
                auto lines = blt::string::split(string, "\n");
                std::string new_source_string;
                for (const auto& line : lines) {
                    if (!line.empty() && !blt::string::contains(line, "\"")) {
                        new_source_string += line;
                        new_source_string += "\n";
                    }
                }
                return new_source_string;
            }
        
        public:
            /**
             * Creates a shader
             * @param vertex vertex shader source or file
             * @param fragment fragment shader source or file
             * @param geometry geometry shader source or file (optional)
             * @param load_as_string load the shader as a string (true) or use the string to load the shader as a file (false)
             */
            shader(const std::string &vertex, const std::string &fragment, const std::string &geometry = "", bool load_as_string = true);
            
            // used to set the location of VAOs to the in variables in opengl shaders.
            void bindAttribute(int attribute, const std::string &name) const;
            
            // used to set location of shared UBOs like the perspective and view matrix
            void setUniformBlockLocation(const std::string &name, int location) const;
            
            // set various data-types.
            inline void setBool(const std::string &name, bool value) {
                glUniform1i(getUniformLocation(name), (int) value);
            }
            
            inline void setInt(const std::string &name, int value) {
                glUniform1i(getUniformLocation(name), value);
            }
            
            inline void setFloat(const std::string &name, float value) {
                glUniform1f(getUniformLocation(name), value);
            }
            
            inline void setMatrix(const std::string &name, blt::mat4x4 &matrix) {
                glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.ptr());
            }
            
            inline void setVec3(const std::string &name, const blt::vec4 &vec) {
                glUniform3f(getUniformLocation(name), vec.x(), vec.y(), vec.z());
            }
            
            inline void setVec4(const std::string &name, const blt::vec4 &vec) {
                // TODO: edit BLT to include a w component
                glUniform4f(getUniformLocation(name), vec.x(), vec.y(), vec.z(), vec[3]);
            }
            
            inline void setVec2(const std::string &name, float x, float y) {
                glUniform2f(getUniformLocation(name), x, y);
            }
            
            inline void setVec3(const std::string &name, float x, float y, float z) {
                glUniform3f(getUniformLocation(name), x, y, z);
            }
            
            inline void setVec4(const std::string &name, float x, float y, float z, float w) {
                glUniform4f(getUniformLocation(name), x, y, z, w);
            }
            
            inline void use() const {
                glUseProgram(programID);
            }
            
            static void updateProjectionMatrix(const blt::mat4x4& projectionMatrix);
            static void updateViewMatrix(const blt::mat4x4& viewMatrix);
            
            ~shader();
    };
}


#endif //FINALPROJECT_GL_H
