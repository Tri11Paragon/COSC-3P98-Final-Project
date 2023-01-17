/*
 * Created by Brett Terpstra 6920201 on 16/01/23.
 * Copyright (c) Brett Terpstra 2023 All Rights Reserved
 */
#include <window/window.h>
#include <GL/glut.h>

glut_window::glut_window(int width, int height) : window(width, height) {
    createGLUTWindow();
}

void glut_window::createWindow() {
    createGLUTWindow();
}

void glut_window::destroyWindow() {
    destroyGLUTWindow();
}

glut_window::~glut_window() {
    destroyGLUTWindow();
}

bool glut_window::setResizeable(bool resizeEnabled) {
    return false;
}

bool glut_window::setWindowSize(int width, int height) {
    m_width = width;
    m_height = height;
    return true;
}

// TODO: a less hacky way of doing this.
blt::window* currentlyActiveWindow = nullptr;

void glut_window_render(){
    if (currentlyActiveWindow != nullptr)
        ((glut_window*)currentlyActiveWindow)->render();
}

void glut_window::createGLUTWindow() {
    glutInit(nullptr, nullptr);
    glutInitWindowSize(m_width, m_height);
    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow("UwU Final Project!");

    currentlyActiveWindow = this;
    glutDisplayFunc(glut_window_render);
}

void glut_window::destroyGLUTWindow() {

}

void glut_window::render() {
    for (const auto& HelloTAThisIsAVeryLargeNameForNoGoodReasonItIsntEvenDescriptive : renderFunctions)
        HelloTAThisIsAVeryLargeNameForNoGoodReasonItIsntEvenDescriptive(this);
}
