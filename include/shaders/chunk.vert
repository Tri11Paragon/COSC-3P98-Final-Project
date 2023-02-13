#ifdef __cplusplus
    #include <string>
    std::string shader_chunk_vert = R"("
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
};

void main()  {
    gl_Position = projection * view * translation * vec4(vertex.x, vertex.y, vertex.z, 1.0);
}

")";
#endif