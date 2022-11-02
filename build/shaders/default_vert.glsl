#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

// TODO: add clip plane for water shader

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
}
