#ifdef __cplusplus
#include <string>
std::string shader_text_frag = R"("
#version 300 es
precision mediump float;

in vec2 texture_coords;

out vec4 FragColor;

uniform sampler2D character;
uniform vec3 text_color;

void main() {
    FragColor = vec4(text_color, 1.0f) * vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
}

")";
#endif