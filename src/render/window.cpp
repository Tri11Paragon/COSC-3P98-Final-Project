/*
 * Created by Brett on 08/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/window.h>
#include <render/gl.h>
#include <blt/std/logging.h>
#include <blt/std/time.h>
#include <unordered_map>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

GLFWwindow* global_window = nullptr;

std::unordered_map<int, bool> key_state{};
// true if the key was pressed this frame.
bool key_pressed_frame = false;
std::unordered_map<int, bool> mouse_state{};
bool mouse_pressed_frame = false;

blt::mat4x4 perspectiveMatrix;
blt::mat4x4 orthographicMatrix;
float fov = 90;

long lastFrame = blt::system::getCurrentTimeNanoseconds();
long delta = 1;

double mouse_dx;
double mouse_last_x;
double mouse_dy;
double mouse_last_y;

#ifdef __EMSCRIPTEN__
    EM_BOOL on_pointerlockchange(int eventType, const EmscriptenPointerlockChangeEvent *event, void *userData) {
        BLT_TRACE("Emscripten pointer lock event status %d", event->isActive);
        if (event->isActive != fp::window::mouseGrabbed())
            glfwSetInputMode(global_window, GLFW_CURSOR, event->isActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        return 0;
    }
#endif


/**
 * GLFW error callback
 * @param error provided by GLFW
 * @param description provided by GLFW
 */
void error_callback(int error, const char* description) {
    // BLT will automatically insert a \n at the end of all macro calls.
    BLT_ERROR("Error: %s\n\n", description);
}

/**
 * Init GLFW, will exit the program if GLFW is unable to init.
 */
inline void initGLFW() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        BLT_FATAL("Failed to init GLFW. Program cannot to continue!\n\n");
        blt::logging::flush();
        std::abort();
    }
}

/**
 * Setup window context. To enable emscriptem / webgl support GL3.0 (GLES 3.0) is preferred.
 */
inline void createContextHints() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API) ;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
        blt::logging::flush();
        std::abort();
    }
}

inline void updateWindowViewport(int width, int height){
    // TODO: remake the matrix implementation. The values are transposed due to a flawed implementation.
    perspectiveMatrix = blt::perspective(fov, (float)width / (float)height, FP_NEAR_PLANE, FP_FAR_PLANE);
    orthographicMatrix = blt::ortho(0, (float)width, 0, (float)height, -5, 5);
    glViewport(0, 0, width, height);
    // make sure we update the global perspective matrix otherwise our rendering is going to look off!
    fp::shader::updateProjectionMatrix(perspectiveMatrix);
    fp::shader::updateOrthographicMatrix(orthographicMatrix);
}

/**
 * Uses sketchy function pointers to handle the important GLFW callbacks.
 */
inline void installCallbacks() {
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
                updateWindowViewport(width, height);
            }
    );
}


void fp::window::init(int width, int height) {
    initGLFW();
    createContextHints();
    createWindow(width, height);
    
    // OpenGL is single threaded. This ensures that the main thread is the one which is allowed to use it.
    glfwMakeContextCurrent(global_window);
    
    installCallbacks();

#ifndef __EMSCRIPTEN__
    int version = gladLoadGLES2(glfwGetProcAddress);
    BLT_INFO("Using GLAD GL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#else
    emscripten_set_pointerlockchange_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, 0, on_pointerlockchange);
    BLT_INFO("Using Emscripten!");
    // we don't want to waste the web browser's resources or cause it to lockup
    glfwSwapInterval(1);
#endif
    
    //glfwSwapInterval(1);
    
    updateWindowViewport(width, height);
    
}

void fp::window::update() {
    double x_pos = 0;
    double y_pos = 0;
    glfwGetCursorPos(global_window, &x_pos, &y_pos);
    
    mouse_dx = x_pos - mouse_last_x;
    mouse_dy = y_pos - mouse_last_y;
    
    mouse_last_x = x_pos;
    mouse_last_y = y_pos;
    
    // reset
    mouse_pressed_frame = false;
    key_pressed_frame = false;
    
    glfwSwapBuffers(global_window);
    // this will cause the key/mouse callbacks which will set the state, which is why it is important to make sure the reset occurs before this.
    glfwPollEvents();
    
    // keep track of how much time has passed between window refreshes
    long currentFrame = blt::system::getCurrentTimeNanoseconds();
    delta = currentFrame - lastFrame;
    lastFrame = currentFrame;
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

void fp::window::setFOV(float new_fov) {
    fov = new_fov;
}

bool fp::window::mouseGrabbed() {
    return glfwGetInputMode(global_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

void fp::window::mouseGrabbed(bool state) {
    glfwSetInputMode(global_window, GLFW_CURSOR, state ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

double fp::window::getFrameDelta() {
    return (double)delta / 1000000000.0;
}

long fp::window::getFrameDeltaRaw() {
    return delta;
}

float fp::window::mouseDX() {
    return (float)mouse_dx;
}

float fp::window::mouseDY() {
    return (float)mouse_dy;
}
