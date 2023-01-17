/*
 * Created by Brett Terpstra 6920201 on 16/01/23.
 * Copyright (c) Brett Terpstra 2023 All Rights Reserved
 */

#ifndef FINAL_PROJECT_WINDOW_H
#define FINAL_PROJECT_WINDOW_H

#include <blt/window/window.h>

class glut_window : public blt::window {
protected:
    void createGLUTWindow();
    void destroyGLUTWindow();
public:
    glut_window() = default;
    glut_window(int width, int height);
    void createWindow() override;
    void destroyWindow() override;
    ~glut_window() override;

    bool setResizeable(bool resizeEnabled) override;
    bool setWindowSize(int width, int height) override;

    void render();
};

#endif //FINAL_PROJECT_WINDOW_H
