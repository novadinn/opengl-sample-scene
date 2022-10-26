#version 330

in VS_OUT {
    vec2 textureCoords;
} vs_in;

out vec4 outColor;

uniform sampler2D guiTexture;

void main(void) {
    outColor = texture(guiTexture, vs_in.textureCoords);
}
