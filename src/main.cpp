#include <blt/std/logging.h>
#include <render/window.h>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#endif

void loop(){
    if (fp::window::isKeyPressed(GLFW_KEY_R)){
        BLT_DEBUG("R Key is pressed!");
        if (fp::window::keyState())
            BLT_TRACE("R Key Single Press!");
    }
    fp::window::update();
}

int main() {
#ifdef __EMSCRIPTEN__
    blt::logging::init(blt::logging::LOG_PROPERTIES{false, true, false, "./"});
#endif
    
    fp::window::init();

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
    
    fp::window::close();
    
    return 0;
}

