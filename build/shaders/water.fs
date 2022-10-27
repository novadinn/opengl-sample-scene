#version 330 core

in VS_OUT {
    vec4 clipSpace;
} vs_in;

out vec4 outColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main() {
    vec2 ndc = (vs_in.clipSpace.xy/vs_in.clipSpace.w)/2.0 + 0.5;
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec4 reflectionColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractionColor = texture(refractionTexture, refractTexCoords);
    
    outColor = mix(reflectionColor, refractionColor, 0.5);
}
