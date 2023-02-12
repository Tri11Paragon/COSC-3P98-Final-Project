/*
 * Created by Brett on 11/02/23.
 * Licensed under GNU General Public License V3.0
 * See LICENSE file for license detail
 */
#include <render/camera.h>
#include <render/gl.h>
#include <blt/std/math.h>
#include <cmath>
#include <blt/std/loader.h>
#include "util/math.h"

blt::vec3 position;
blt::vec3 rotation;

const float MAX_SPEED = 100;
const float DEFAULT_SPEED = 50;
float speed = 0;

float rotation_speed = 3;

void updateCursorGrabbedState(){
#ifndef __EMSCRIPTEN__
    if (fp::window::isKeyPressed(GLFW_KEY_ESCAPE) && fp::window::keyState())
        fp::window::mouseGrabbed(!fp::window::mouseGrabbed());
#else
    //fp::window::mouseGrabbed(true);
#endif
}

void updateCameraRotation(){
    if (!fp::window::mouseGrabbed())
        return;
    
    rotation[1] += (float)(-fp::window::mouseDY() * fp::window::getFrameDelta() * rotation_speed);
    rotation[2] += (float)(-fp::window::mouseDX() * fp::window::getFrameDelta() * rotation_speed);
    
    if (rotation[2] > 360)
        rotation[2] = 0;
    if (rotation[2] < 0)
        rotation[2] = 360;
    if (rotation[1] > 90)
        rotation[1] = 90;
    if (rotation[1] < -90)
        rotation[1] = -90;
}

void updateViewMatrix(){
    blt::mat4x4 viewMatrix;
    
    float pitch = degreesToRadian(rotation[1]);
    float yaw = degreesToRadian(rotation[2]);
    
    float cosPitch = std::cos(pitch);
    float cosYaw = std::cos(yaw);
    float sinPitch = std::sin(pitch);
    float sinYaw = std::sin(yaw);
    
    auto x = blt::vec4{cosYaw, 0, -sinYaw}; // forward
    auto y = blt::vec4{sinYaw * sinPitch, cosPitch, cosYaw * sinPitch}; // right
    auto z = blt::vec4{sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw}; // up
    
    viewMatrix.m00(float(x.x()));
    viewMatrix.m01(float(x.y()));
    viewMatrix.m02(float(x.z()));
    viewMatrix.m03(float(x.w()));
    
    viewMatrix.m10(float(y.x()));
    viewMatrix.m11(float(y.y()));
    viewMatrix.m12(float(y.z()));
    viewMatrix.m13(float(y.w()));
    
    viewMatrix.m20(float(z.x()));
    viewMatrix.m21(float(z.y()));
    viewMatrix.m22(float(z.z()));
    viewMatrix.m23(float(z.w()));
    
    // view matrix are inverted, dot product to simulate translate matrix multiplication
    viewMatrix.m03(-float(blt::vec<4>::dot(x, blt::vec<4>{position.x(), position.y(), position.z(), 0})));
    viewMatrix.m13(-float(blt::vec<4>::dot(y, blt::vec<4>{position.x(), position.y(), position.z(), 0})));
    viewMatrix.m23(-float(blt::vec<4>::dot(z, blt::vec<4>{position.x(), position.y(), position.z(), 0})));
    viewMatrix.m33(1);
    
    fp::shader::updateViewMatrix(viewMatrix);
}

void fp::camera::update() {
    updateCursorGrabbedState();
    updateCameraRotation();
    if (window::isKeyPressed(GLFW_KEY_LEFT_CONTROL)){
        speed = MAX_SPEED;
    } else if (window::isKeyPressed(GLFW_KEY_LEFT_ALT))
        speed = DEFAULT_SPEED / 8;
    else
        speed = DEFAULT_SPEED;
    
    float currentSpeedX = 0;
    float currentSpeedY = 0;
    float currentSpeedZ = 0;
    
    if (window::isKeyPressed(GLFW_KEY_W))
        currentSpeedZ = speed;
    else if (window::isKeyPressed(GLFW_KEY_S))
        currentSpeedZ = -speed;
    
    if (window::isKeyPressed(GLFW_KEY_A))
        currentSpeedX = -speed;
    else if (window::isKeyPressed(GLFW_KEY_D))
        currentSpeedX = speed;
    
    if (window::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        currentSpeedY = -speed;
    else if (window::isKeyPressed((GLFW_KEY_SPACE)))
        currentSpeedY = speed;
    else
        currentSpeedY = 0;
    
    float yawRadians = degreesToRadian(rotation[2]);
    
    // https://www.desmos.com/calculator/gccgtjqpcy
    // visual description of this equation, Y is Z here and R is the yaw
    float dx = -currentSpeedZ * sinf(yawRadians) + currentSpeedX * cosf(yawRadians);
    float dy = currentSpeedY;
    float dz = -currentSpeedZ * cosf(yawRadians) + -currentSpeedX * sinf(yawRadians);
    
    position[0] += (float)(dx * window::getFrameDelta());
    position[1] += (float)(dy * window::getFrameDelta());
    position[2] += (float)(dz * window::getFrameDelta());
    
    updateViewMatrix();
}
