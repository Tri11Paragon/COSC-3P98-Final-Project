#ifdef __cplusplus
#include <string>
std::string shader_plane_vert = R"("
#version 300 es
precision mediump float;

layout (location = 0) in vec3 vertex;

uniform mat4 translation;

layout (std140) uniform StandardMatrices
{
    mat4 projection;
    mat4 view;
// projection view matrix
    mat4 pvm;
// orthographic projection matrix
    mat4 orthographic;
};

void main() {
    gl_Position = pvm * vec4(vertex, 1.0);
}


")";
#endif