#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec4 plane;

void main()
{
    vec4 worldPosition = model * vec4(position, 1.0);

    gl_ClipDistance[0] = dot(worldPosition, plane);
    
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(worldPosition);
    TexCoords = texCoords;
    gl_Position = projection * view * worldPosition;
}
