#version 330 core

in vec3 TexCoords;

out vec4 FragColor;

uniform samplerCube dayTexture;
uniform samplerCube nightTexture;
uniform float blendFactor;

void main()
{
    vec4 dayColor = texture(dayTexture, TexCoords);
    vec4 nightColor = texture(nightTexture, TexCoords);
    vec4 finalColor = mix(dayColor, nightColor, blendFactor);
    FragColor = finalColor;
}
