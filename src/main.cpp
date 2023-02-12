#include <blt/std/logging.h>
#include <render/window.h>
#include <render/gl.h>
#include <memory>

#include <shaders/chunk.frag>
#include <shaders/chunk.vert>
#include "render/camera.h"
#include "world/chunk/chunk.h"


#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#endif

fp::shader* chunk_shader;
fp::chunk* chunk;

void loop(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    chunk_shader->use();
    chunk->update();
    chunk->render(*chunk_shader);

    if (fp::window::isKeyPressed(GLFW_KEY_R)){
        BLT_DEBUG("R Key is pressed!");
        if (fp::window::keyState())
            BLT_TRACE("R Key Single Press!");
    }
    fp::camera::update();
    fp::window::update();
}

int main() {
    auto logging_properties = blt::logging::LOG_PROPERTIES{true, true, true, "./"};
    logging_properties.m_logFullPath = true;
    
#ifdef __EMSCRIPTEN__
    logging_properties = blt::logging::LOG_PROPERTIES{false, true, false, "./"};
#endif
    
    blt::logging::init(logging_properties);
    
    fp::window::init();
    
    chunk_shader = new fp::shader(shader_chunk_vert, shader_chunk_frag);
    chunk = new fp::chunk();
    
    chunk->setBlock(0, 0, 0, 1);
    chunk->setBlock(2, 2, 2, 1);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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

    delete(chunk_shader);
    delete(chunk);
    
    fp::window::close();
    
    return 0;
}

