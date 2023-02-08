/*
 * Created by Brett Terpstra 6920201 on 16/01/23.
 * Copyright (c) Brett Terpstra 2023 All Rights Reserved
 */

#ifndef FINAL_PROJECT_WINDOW_H
#define FINAL_PROJECT_WINDOW_H

// emscripten provides its own gl bindings.
#ifndef __EMSCRIPTEN__
    #include <glad/gles2.h>
#endif
#include <GLFW/glfw3.h>
#include <blt/std/math.h>

namespace fp::window {
    void init(int width = 1440, int height = 720);
    
    void update();
    
    void close();
    
    bool isCloseRequested();
    
    GLFWwindow* getWindow();
    
    bool isKeyPressed(int key);
    
    bool isMousePressed(int button);
    
    bool mouseState();
    bool keyState();
    
    const blt::mat4x4& getPerspectiveMatrix();
}

#endif //FINAL_PROJECT_WINDOW_H
