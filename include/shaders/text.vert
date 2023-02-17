#ifdef __cplusplus
#include <string>
std::string shader_text_vert = R"("
#version 300 es
precision mediump float;

layout (location = 0) in vec4 vertex;

out vec2 texture_coords;

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

void main()  {
    gl_Position = orthographic * translation * vec4(vertex.xy, 0.0, 1.0);
    texture_coords = vertex.zw;
}

")";
#endif