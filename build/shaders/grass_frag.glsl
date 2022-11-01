#version 330 core

in GS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} gs_in;

out vec4 color;

const vec4 bottomColor = vec4(0.2, 0.7, 0.01, 1.0);
const vec4 topColor = vec4(0.8, 1.0, 0.5, 1.0);

void main() {
    color = mix(bottomColor, topColor, gs_in.TexCoords.y);
}

