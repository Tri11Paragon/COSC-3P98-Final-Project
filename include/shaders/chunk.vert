#ifdef __cplusplus
    #include <string>
    std::string shader_chunk_vert = R"("
#version 300 es
precision mediump float;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 texture_coord;

out vec2 uv;
out float index;

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
    index = texture_coord.z;
    gl_Position = projection * view * translation * vec4(vertex.x, vertex.y, vertex.z, 1.0);
    uv = texture_coord.xy;
}

")";
#endif