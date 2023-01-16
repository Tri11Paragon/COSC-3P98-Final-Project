/*
 * Created by Brett Terpstra 6920201 on 16/01/23.
 * Copyright (c) Brett Terpstra 2023 All Rights Reserved
 */
#include <window/window.h>

glut_window::glut_window() {

}

void glut_window::createWindow() {

}

void glut_window::destroyWindow() {

}

glut_window::~glut_window() {

}

bool glut_window::setResizeable(bool resizeEnabled) {
    return false;
}

bool glut_window::setWindowSize(int width, int height) {
    return false;
}

int glut_window::getWidth() {
    return 0;
}

int glut_window::getHeight() {
    return 0;
}

bool glut_window::isKeyDown(int key) {
    return false;
}

bool glut_window::isMouseDown(int button) {
    return false;
}

void glut_window::registerKeyListener(std::function<(void) (bool)...> listener) {

}

void glut_window::registerMouseListener(std::function<(void) (bool)...> listener) {

}
