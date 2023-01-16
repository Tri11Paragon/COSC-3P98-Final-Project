/*
 * Created by Brett Terpstra 6920201 on 16/01/23.
 * Copyright (c) Brett Terpstra 2023 All Rights Reserved
 */

#ifndef FINAL_PROJECT_WINDOW_H
#define FINAL_PROJECT_WINDOW_H

#include <blt/window/window.h>

class glut_window : public blt::window {
private:

public:
    glut_window();
    virtual void createWindow() override;
    virtual void destroyWindow() override;
    virtual ~glut_window();

    virtual bool setResizeable(bool resizeEnabled) override;
    virtual bool setWindowSize(int width, int height) override;
    virtual int getWidth() override;
    virtual int getHeight() override;

    virtual bool isKeyDown(int key) override;
    virtual bool isMouseDown(int button) override;
    // Function signature is window pointer to this, key press, pressed/released (true/false)
    virtual void registerKeyListener(std::function<void(window*, int, bool)> listener) override;
    // Function signature is window pointer to this, mouse button press, pressed/released (true/false)
    virtual void registerMouseListener(std::function<void(window*, int, bool)> listener) override;
};

#endif //FINAL_PROJECT_WINDOW_H
