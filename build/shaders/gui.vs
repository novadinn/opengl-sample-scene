#version 330 core

layout (location = 0) in vec2 inPosition;

out VS_OUT {
    vec2 textureCoords;
} vs_out;

uniform mat4 model;

void main(void) {
    vs_out.textureCoords = vec2((inPosition.x + 1.0) / 2.0, 1 - (inPosition.y + 1.0) / 2.0);
    gl_Position = model * vec4(inPosition, 0.0, 1.0);
}
