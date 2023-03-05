#ifdef __cplusplus
    #include <string>
    std::string shader_chunk_frag = R"("
#version 300 es
precision mediump float;

out vec4 FragColor;

in vec2 uv;
in float index;

uniform mediump sampler2DArray texturep_palette;

void main() {
/** if (gl_fragcoord * gl_fragcoord < vec2(5, 5))
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
else
    discard;*/
FragColor = texture(texturep_palette, vec3(uv, index));
}

")";
#endif