#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D sprite;

void main() {
    color = texture(sprite, TexCoord);
}
