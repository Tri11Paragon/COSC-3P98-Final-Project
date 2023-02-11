#include <blt/std/logging.h>
#include <render/window.h>
#include <render/gl.h>
#include <memory>

#include <shaders/test.frag>
#include <shaders/test.vert>
#include "render/camera.h"


#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#endif

const float scale = 1;

float vertices[] = {
        0.5f  * scale,  0.5f  * scale, -2.0f,  // top right
        0.5f  * scale, -0.5f  * scale, -2.0f,  // bottom right
        -0.5f * scale, -0.5f * scale, -2.0f,  // bottom left
        -0.5f * scale,  0.5f * scale, -2.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};

fp::shader* activeShader;
fp::VAO* activeVAO;

void loop(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    activeShader->use();
    activeVAO->bind();
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glDisableVertexAttribArray(0);
    if (fp::window::isKeyPressed(GLFW_KEY_R)){
        BLT_DEBUG("R Key is pressed!");
        if (fp::window::keyState())
            BLT_TRACE("R Key Single Press!");
    }
    fp::camera::update();
    fp::window::update();
}

int main() {
#ifdef __EMSCRIPTEN__
    blt::logging::init(blt::logging::LOG_PROPERTIES{false, true, false, "./"});
#endif
    
    fp::window::init();
    
    activeShader = new fp::shader(shader_test_vert, shader_test_frag);
    activeVAO = new fp::VAO();
    activeVAO->bindVBO(new fp::VBO{fp::ARRAY_BUFFER, vertices, sizeof(vertices)}, 0, 3);
    activeVAO->bindElementVBO(new fp::VBO{fp::ELEMENT_BUFFER, indices, sizeof(indices)});

#ifdef __EMSCRIPTEN__
    /*
     * "setting 0 or a negative value as the fps will use the browser’s requestAnimationFrame mechanism to call the main loop function.
     * This is HIGHLY recommended if you are doing rendering, as the browser’s requestAnimationFrame will
     * make sure you render at a proper smooth rate that lines up properly with the browser and monitor."
     * https://emscripten.org/docs/api_reference/emscripten.h.html
     */
    emscripten_set_main_loop(loop, 60, true);
#else
    while(!fp::window::isCloseRequested())
        loop();
#endif

    delete(activeShader);
    delete(activeVAO);
    
    fp::window::close();
    
    return 0;
}

