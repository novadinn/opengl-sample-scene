#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec3 color;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec3 color;
} vs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec4 plane;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0);
    gl_ClipDistance[0] = dot(worldPosition, plane);

    vs_out.fragPos = vec3(worldPosition);
    vs_out.normal = mat3(transpose(inverse(model))) * normal;
    vs_out.color = color;
    gl_Position = projection * view * worldPosition;
}
