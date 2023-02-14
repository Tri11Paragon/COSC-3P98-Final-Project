#include <blt/std/logging.h>
#include <render/window.h>
#include <render/gl.h>
#include <memory>

#include <shaders/chunk.frag>
#include <shaders/chunk.vert>
#include "render/camera.h"
#include "world/chunk/world.h"


#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#endif

fp::shader* chunk_shader;
fp::world* world;

void loop(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    world->update();
    world->render(*chunk_shader);
    
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
    fp::registry::registerDefaultBlocks();
    
    chunk_shader = new fp::shader(shader_chunk_vert, shader_chunk_frag);
    world = new fp::world();
    
    for (int i = 1; i < CHUNK_SIZE; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 5; k < CHUNK_SIZE; k++)
                world->setBlock({i,j,k}, 1);
    world->setBlock({-2, 2, 2}, 1);
    world->setBlock({-2, 2, -2}, 1);
    world->setBlock({2, 2, -2}, 1);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

#ifdef __EMSCRIPTEN__
    /*
     * "setting 0 or a negative value as the fps will use the browser’s requestAnimationFrame mechanism to call the main loop function.
     * This is HIGHLY recommended if you are doing rendering, as the browser’s requestAnimationFrame will
     * make sure you render at a proper smooth rate that lines up properly with the browser and monitor."
     * https://emscripten.org/docs/api_reference/emscripten.h.html
     */
    emscripten_set_main_loop(loop, 0, true);
#else
    while(!fp::window::isCloseRequested())
        loop();
#endif

    delete(chunk_shader);
    delete(world);
    
    fp::window::close();
    
    return 0;
}

