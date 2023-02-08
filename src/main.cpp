#include <blt/std/logging.h>
#include <render/window.h>

int main() {
    
    fp::window::init();
    
    while(!fp::window::isCloseRequested()){
        if (fp::window::isKeyPressed(GLFW_KEY_R)){
            BLT_DEBUG("R Key is pressed!");
            if (fp::window::keyState())
                BLT_TRACE("R Key Single Press!");
        }
        fp::window::update();
    }
    
    fp::window::close();
    
    return 0;
}

