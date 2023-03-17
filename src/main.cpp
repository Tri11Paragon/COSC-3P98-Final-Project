#include <blt/std/logging.h>
#include <blt/profiling/profiler.h>
#include <render/window.h>
#include <render/gl.h>
#include <render/ui/graphics.h>
#include <render/ui/debug.h>
#include <memory>

#include <shaders/chunk.frag>
#include <shaders/chunk.vert>
#include "render/camera.h"
#include "world/world.h"
#include "util/settings.h"
#include <util/math.h>


#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#endif

fp::shader* chunk_shader;
fp::world* world;
fp::renderer* renderer;

void loop(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    world->update();
    world->render(*chunk_shader);
    
    //fp::text::drawText("Hello There", 0, 0, fp::text::FONT_18, {0,0,0, 1.0});
    
    fp::camera::update();
    fp::debug::render();
    fp::graphics::render();
    fp::window::update();
}

int main() {
    auto logging_properties = blt::logging::LOG_PROPERTIES{true, true, false, "./"};
    logging_properties.m_logFullPath = true;
    
#ifdef __EMSCRIPTEN__
    logging_properties = blt::logging::LOG_PROPERTIES{false, true, false, "./"};
#endif
    
    blt::logging::init(logging_properties);
    fp::settings::load("settings.txt");
    
    fp::window::init();
    
    renderer = new fp::renderer();
    fp::graphics::init(*renderer);
    // textures must come first as blocks will require the IDs
    fp::registry::registerDefaultTextures();
    fp::registry::registerDefaultBlocks();
    
    chunk_shader = renderer->createShader(fp::shader(shader_chunk_vert, shader_chunk_frag));
    world = new fp::world();
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
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
    
    delete(world);
    delete(renderer);
    
    /** !! MUST BE CALLED HERE OTHERWISE glDeleteTextures WILL BE CALLED AFTER THE GL CONTEXT IS DESTROYED! !! **/
    fp::registry::cleanup();
    fp::graphics::cleanup();
    fp::window::close();
    fp::settings::save("settings.txt");
    
    return 0;
}

