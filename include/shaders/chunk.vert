#ifdef __cplusplus
    #include <string>
    std::string shader_chunk_vert = R"("
#version 300 es
precision mediump float;

//layout (location = 0) in vec3 vertex;
//layout (location = 1) in vec3 texture_coord;

const vec2 UV_COORDS[4] = vec2[4](
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 1)
);

layout (location = 0) in float data;

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
    const int texture_index_loc = 32 - 8;
    const int uv_index_loc = texture_index_loc - 2;
    const int x_coord_loc = uv_index_loc - 6;
    const int y_coord_loc = x_coord_loc - 6;
    const int z_coord_loc = y_coord_loc - 6;

    int idata = floatBitsToInt(data);

    int texture_index = idata >> texture_index_loc;
    int uv_index = ((idata >> uv_index_loc) & 0x3);
    float x_coord = float((idata >> x_coord_loc) & 0x3F);
    float y_coord = float((idata >> y_coord_loc) & 0x3F);
    float z_coord = float((idata >> z_coord_loc) & 0x3F);

    index = float(texture_index);
    gl_Position = projection * view * translation * vec4(-0.5 + x_coord, -0.5 + y_coord, -0.5 + z_coord, 1.0);
    uv = UV_COORDS[uv_index].xy;
}

")";
#endif