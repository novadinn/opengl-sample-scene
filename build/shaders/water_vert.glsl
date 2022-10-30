#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out VS_OUT {
    vec4 clipSpace;
    vec2 texCoords;
    vec3 toCameraVector;
    vec3 fromLightVector;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;

const float tiling = 1.0;

void main() {
    // vs_out.Normal = mat3(transpose(inverse(model))) * inNormal;
    // vs_out.FragPos = vec3(model * vec4(inPosition, 1.0));
    vec4 worldPosition = model * vec4(inPosition, 1.0);
    vs_out.clipSpace = projection * view * worldPosition;
    vs_out.texCoords = vec2(inTexCoords.x / 2.0 + 0.0, inTexCoords.y / 2.0 + 0.5) * tiling;
    vs_out.toCameraVector = cameraPosition - worldPosition.xyz;
    vs_out.fromLightVector = worldPosition.xyz - lightPosition;
    gl_Position = vs_out.clipSpace;
}
