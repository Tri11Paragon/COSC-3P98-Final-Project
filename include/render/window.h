/*
 * Created by Brett Terpstra 6920201 on 16/01/23.
 * Copyright (c) Brett Terpstra 2023 All Rights Reserved
 */

#ifndef FINAL_PROJECT_WINDOW_H
#define FINAL_PROJECT_WINDOW_H

#ifndef FP_FAR_PLANE
    #define FP_FAR_PLANE 1000.0f
    #define FP_NEAR_PLANE 0.1f
#endif

// emscripten provides its own gl bindings.
#ifndef __EMSCRIPTEN__
    #include <glad/gles2.h>
#else
    #include <GLES2/gl2.h>
    #include <emscripten.h>
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#endif
#include <GLFW/glfw3.h>
#include <blt/math/math.h>

namespace fp::window {
    /**
     * Handles all the init setup for creating a GLFW window.
     * @param width width of the window
     * @param height height of the window
     */
    void init(int width = 1440, int height = 720);
    
    void update();
    
    void close();
    
    bool isCloseRequested();
    
    GLFWwindow* getWindow();
    
    bool isKeyPressed(int key);
    
    bool isMousePressed(int button);
    
    bool mouseState();
    bool keyState();
    
    bool mouseGrabbed();
    void mouseGrabbed(bool state);
    
    float mouseDX();
    float mouseDY();
    
    // seconds
    double getFrameDelta();
    // nanoseconds
    long getFrameDeltaRaw();
    
    const blt::mat4x4& getPerspectiveMatrix();
    
    void setFOV(float new_fov);
}

#endif //FINAL_PROJECT_WINDOW_H
