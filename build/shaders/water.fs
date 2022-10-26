#version 330 core

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
} vs_in;

out vec4 outColor;

void main() {
    outColor = vec4(1.0);
}
