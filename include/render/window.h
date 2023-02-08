/*
 * Created by Brett Terpstra 6920201 on 16/01/23.
 * Copyright (c) Brett Terpstra 2023 All Rights Reserved
 */

#ifndef FINAL_PROJECT_WINDOW_H
#define FINAL_PROJECT_WINDOW_H

#include <glad/gles2.h>
#include <GLFW/glfw3.h>
#include <blt/std/math.h>

namespace fp::window {
    static void init(int width = 1440, int height = 720);
    
    static void update();
    
    static void close();
    
    static bool isCloseRequested();
    
    static GLFWwindow* getWindow();
    
    static bool isKeyPressed(int key);
    
    static bool isMousePressed(int button);
    
    const static blt::mat4x4& getPerspectiveMatrix();
}

#endif //FINAL_PROJECT_WINDOW_H
