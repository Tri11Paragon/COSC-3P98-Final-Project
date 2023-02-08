/*
 * Created by Brett on 08/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/window.h>
#include <blt/std/logging.h>
#include <unordered_map>

GLFWwindow* global_window = nullptr;
std::unordered_map<int, bool> key_state{};
// true if the key was pressed this frame.
bool key_pressed_frame = false;
std::unordered_map<int, bool> mouse_state{};
bool mouse_pressed_frame = false;
blt::mat4x4 perspectiveMatrix;

/**
 * GLFW error callback
 * @param error provided by GLFW
 * @param description provided by GLFW
 */
void error_callback(int error, const char* description) {
    // BLT will automatically insert a \n at the end of all macro calls.
    BLT_ERROR("Error: %s", description);
}

/**
 * Init GLFW, will exit the program if GLFW is unable to init.
 */
inline void initGLFW() {
    if (!glfwInit()) {
        BLT_FATAL("Failed to init GLFW. Program cannot to continue!");
        std::abort();
    }
    glfwSetErrorCallback(error_callback);
}

/**
 * Setup window context. To enable emscriptem / webgl support GL2.0 (GLES 2.0) is preferred.
 */
inline void createContext() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

/**
 * Create the actual window of width,height exiting if GLFW fails to create the window.
 */
inline void createWindow(int width, int height) {
    global_window = glfwCreateWindow(width, height, "COSC 3P98 Final Project", nullptr, nullptr);
    if (!global_window) {
        BLT_FATAL("Unable to create GLFW window, see error log for more details.");
        glfwTerminate();
        std::abort();
    }
}

/**
 * Uses sketchy function pointers to handle the important GLFW callbacks.
 */
inline void initCallbacks() {
    glfwSetKeyCallback(
            global_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
                // pressed state has GLFW_PRESS and GLFW_REPEAT. GLFW_RELEASE is used instead.
                key_state[key] = (action != GLFW_RELEASE);
                key_pressed_frame = true;
            }
    );
    glfwSetMouseButtonCallback(
            global_window, [](GLFWwindow* window, int button, int action, int mods) -> void {
                mouse_state[button] = (action != GLFW_RELEASE);
                mouse_pressed_frame = true;
            }
    );
    glfwSetFramebufferSizeCallback(
            global_window, [](GLFWwindow* window, int width, int height) -> void {
                glViewport(0, 0, width, height);
            }
    );
}

/**
 * Handles all the init setup for creating a GLFW window.
 * @param width width of the window
 * @param height height of the window
 */
void fp::window::init(int width, int height) {
    initGLFW();
    createContext();
    createWindow(width, height);
    glfwMakeContextCurrent(global_window);
    initCallbacks();

#ifndef __EMSCRIPTEN__
    int version = gladLoadGLES2(glfwGetProcAddress);
    BLT_INFO("Using GLAD GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#endif

}

void fp::window::update() {
    // reset
    mouse_pressed_frame = false;
    key_pressed_frame = false;
    
    glfwSwapBuffers(global_window);
    // this will cause the key/mouse callbacks which will set the state, which is why it is important to make sure the reset occurs before this.
    glfwPollEvents();
}

void fp::window::close() {
    glfwDestroyWindow(global_window);
    glfwTerminate();
}

bool fp::window::isCloseRequested() {
    return glfwWindowShouldClose(global_window);
}

GLFWwindow* fp::window::getWindow() {
    return global_window;
}

bool fp::window::isKeyPressed(int key) {
    return key_state[key];
}

bool fp::window::isMousePressed(int button) {
    return mouse_state[button];
}

const blt::mat4x4& fp::window::getPerspectiveMatrix() {
    return perspectiveMatrix;
}

bool fp::window::mouseState() {
    return mouse_pressed_frame;
}

bool fp::window::keyState() {
    return key_pressed_frame;
}
