#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube dayTexture;

void main()
{
    FragColor = texture(dayTexture, TexCoords);
}
