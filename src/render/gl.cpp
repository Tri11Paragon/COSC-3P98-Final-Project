/*
 * Created by Brett on 08/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#ifndef __EMSCRIPTEN__
#define GLAD_GLES2_IMPLEMENTATION
#endif

#include <render/gl.h>
#include <blt/std/logging.h>
#include <blt/std/memory.h>
#include <blt/std/loader.h>

namespace fp::_static {
    bool matricesUBOCreated = false;
    unsigned int matricesUBO = 0;
    blt::mat4x4 projectionMatrix {};
    blt::mat4x4 viewMatrix {};
    
    inline void createMatricesUBO(){
        if (matricesUBOCreated)
            return;
        glGenBuffers(1, &matricesUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        // 3 matrices stored [Perspective (64), View (64), pvm (64)]
        glBufferData(GL_UNIFORM_BUFFER, sizeof(blt::mat4x4) * 3, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
        // set the main matrices UBO to be in position 0. This will always be reserved for this purpose.
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
    }
    
    inline void updatePerspectiveUBO(){
        createMatricesUBO();
        // upload only the perspective since it changes rarely
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(blt::mat4x4), projectionMatrix.ptr());
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    inline void updateViewUBO(){
        // the UBO will have been created by now since the perspective matrix is updated on window creation.
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        auto pvm = projectionMatrix * viewMatrix;
        // by writing as offsets into one single buffer we can avoid overwriting the perspective matrix, and save us two more buffers
        // since writing both at the same time is faster than binding a whole separate buffer. (Remember this gets ran once per frame!)
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(blt::mat4x4), sizeof(blt::mat4x4), viewMatrix.ptr());
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(blt::mat4x4) * 2, sizeof(blt::mat4x4), pvm.ptr());
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

namespace fp {
    VAO::VAO() {
        glGenVertexArrays(1, &vaoID);
    }
    
    VAO::~VAO() {
        for (VBO* vbo : VBOs)
            delete (vbo);
        glDeleteVertexArrays(1, &vaoID);
    }
    
    void VAO::bindVBO(VBO* vbo, int attribute_number, int coordinate_size, GLenum type, int stride, long offset) {
        bind();
        vbo->bind();
        
        glVertexAttribPointer(
                attribute_number, coordinate_size, type, GL_FALSE, stride < 0 ? int(coordinate_size * sizeof(float)) : stride, (void*) offset
        );
        glEnableVertexAttribArray(attribute_number);
        
        VBOs.push_back(vbo);
    }
    
    void VAO::bindElementVBO(VBO* vbo) {
        bind();
        vbo->bind();
        VBOs.push_back(vbo);
    }
    
    
    unsigned int shader::createShader(const std::string& source, int type) {
        const char* shader_code = source.c_str();
        // creates a Shader
        unsigned int shaderID = glCreateShader(type);
        // loads the shader code for later complication and uploading into the graphics card
        // TODO: defines can be added here by sending them as additional strings. No need to edit the source string
        glShaderSource(shaderID, 1, &shader_code, nullptr);
        // Compile it
        glCompileShader(shaderID);
        
        // make sure there are no errors in the compilation. If there is then print out information pertaining to the error.
        // the actual log is highly dependent on the platform this is being run from, so we cannot make any assumptions about the issue.
        // the TODO: maybe find a way of lexing the output to give suggestions about fixing the error? default error messages can be unhelpful at times.
        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            int log_length = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &log_length);
            
            // scoped buffers will delete their memory when they go out of scope. A benefit of using BLT
            blt::scoped_buffer<GLchar> infoLog{static_cast<unsigned long>(log_length + 1)};
            
            glGetShaderInfoLog(shaderID, log_length + 1, nullptr, infoLog.buffer);
            auto shader_type_str = (type == GL_VERTEX_SHADER ? "Vertex Shader" : type == GL_FRAGMENT_SHADER ? "Fragment Shader" : "Other Shader");
            BLT_ERROR("--- --- --- --- --- --- --- --- ---");
            BLT_ERROR("Unable to compile shader of type %s\nShader source:", shader_type_str);
            BLT_ERROR(source);
            BLT_ERROR("I have an log of %d length", log_length);
            BLT_ERROR(infoLog.buffer);
            BLT_ERROR("--- --- --- --- --- --- --- --- ---");
        }
        return shaderID;
    }
    
    shader::shader(const std::string& vertex, const std::string& fragment, const std::string& geometry, bool load_as_string) {
        // load shader sources
        bool load_geometry = !geometry.empty();
        std::string vertex_source = vertex;
        std::string fragment_source = fragment;
        std::string geometry_source = geometry;
        if (!load_as_string){
            // BLT provides a recursive file loader for glsl shaders. It's pretty much just a recursive function looking for include statements.
            vertex_source = blt::fs::loadShaderFile(vertex);
            fragment_source = blt::fs::loadShaderFile(fragment);
            if (load_geometry)
                geometry_source = blt::fs::loadShaderFile(geometry);
        } else {
            vertex_source = removeEmptyFirstLines(vertex_source);
            fragment_source = removeEmptyFirstLines(fragment_source);
            geometry_source = removeEmptyFirstLines(geometry_source);
        }
        
        // create the shaders
        vertexShaderID = createShader(vertex_source, GL_VERTEX_SHADER);
        fragmentShaderID = createShader(fragment_source, GL_FRAGMENT_SHADER);
        if (load_geometry)
            BLT_ERROR("Unable to load geometry shader because webgl doesn't support it!");
        
        // bind them to a program
        programID = glCreateProgram();
        // attach the loaded shaders to the Shader program
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        if (load_geometry)
            glAttachShader(programID, geometryShaderID);
        // link and make sure that our program is valid.
        glLinkProgram(programID);
        
        GLint success;
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            int log_length = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &log_length);
        
            // scoped buffers will delete their memory when they go out of scope. A benefit of using BLT
            blt::scoped_buffer<GLchar> infoLog{static_cast<unsigned long>(log_length + 1)};
        
            glGetProgramInfoLog(programID, log_length + 1, nullptr, infoLog.buffer);
            BLT_ERROR("--- --- --- --- --- --- --- --- ---");
            BLT_ERROR("Unable to link program of ID: %", programID);
            BLT_ERROR(vertex_source);
            BLT_ERROR(fragment_source);
            BLT_ERROR(geometry_source);
            BLT_ERROR("I have an log of %d length", log_length);
            BLT_ERROR(infoLog.buffer);
            BLT_ERROR("--- --- --- --- --- --- --- --- ---");
        }
        
        glValidateProgram(programID);
        use();
        setUniformBlockLocation("StandardMatrices", 0);
        glUseProgram(0);
        
    }
    
    void shader::bindAttribute(int attribute, const std::string &name) const {
        use();
        glBindAttribLocation(programID, attribute, name.c_str());
    }
    
    void shader::setUniformBlockLocation(const std::string &name, int location) const {
        use();
        glUniformBlockBinding(programID, glGetUniformBlockIndex(programID, name.c_str()), location);
    }
    
    shader::~shader() {
        glUseProgram(0);
        // remove all the shaders from the program
        glDetachShader(programID, vertexShaderID);
        if (geometryShaderID)
            glDetachShader(programID, geometryShaderID);
        if (tessellationShaderID)
            glDetachShader(programID, tessellationShaderID);
        glDetachShader(programID, fragmentShaderID);
    
        // delete the shaders
        glDeleteShader(vertexShaderID);
        if (geometryShaderID)
            glDeleteShader(geometryShaderID);
        if (tessellationShaderID)
            glDeleteShader(tessellationShaderID);
        glDeleteShader(fragmentShaderID);
    
        // delete the Shader program
        glDeleteProgram(programID);
    }
    
    void shader::updateProjectionMatrix(const blt::mat4x4& projectionMatrix) {
        fp::_static::projectionMatrix = projectionMatrix;
        fp::_static::updatePerspectiveUBO();
    }
    
    void shader::updateViewMatrix(const blt::mat4x4& viewMatrix) {
        fp::_static::viewMatrix = viewMatrix;
        fp::_static::updateViewUBO();
    }
}
