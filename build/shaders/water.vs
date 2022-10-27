#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out VS_OUT {
    vec4 clipSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // vs_out.Normal = mat3(transpose(inverse(model))) * inNormal;
    // vs_out.FragPos = vec3(model * vec4(inPosition, 1.0));
    // vs_out.TexCoords = inTexCoords;
    vs_out.clipSpace = projection * view * model * vec4(inPosition, 1.0);
    gl_Position = vs_out.clipSpace;
}
