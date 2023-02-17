#ifdef __cplusplus
#include <string>
std::string shader_text_frag = R"("
#version 300 es
precision mediump float;

in vec2 texture_coords;

out vec4 FragColor;

uniform sampler2D character;
uniform vec4 text_color;
uniform vec4 background_color;

void main() {
    FragColor = text_color * vec4(1.0, 1.0, 1.0, texture(character, texture_coords).r);
    //FragColor = vec4(0.0, 1.0, 0.0, 1.0) * vec4(1.0, 1.0, 1.0, texture(character, texture_coords).r);
}

")";
#endif