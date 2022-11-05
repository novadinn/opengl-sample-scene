#version 330 core

in VS_OUT {
    vec4 clipSpace;
    vec2 texCoords;
    vec3 toCameraVector;
} vs_in;

out vec4 outColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float moveFactor;

uniform float waveStrength;
uniform float shineDamper;
uniform float reflectivity;

uniform float near;
uniform float far;

const vec4 waterColor = vec4(0.0, 0.3, 0.5, 1.0);
const float alphaDivisor = 0.5;
const float distortionDivisor = 20.0;
const float colorMixAmount = 0.5;

void main() {
    vec2 ndc = (vs_in.clipSpace.xy/vs_in.clipSpace.w)/2.0 + 0.5;
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);

    float depth = texture(depthMap, refractTexCoords).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    float waterDepth = floorDistance - waterDistance;
    
    vec2 distortedTexCoords = texture(dudvMap, vec2(vs_in.texCoords.x + moveFactor, vs_in.texCoords.y)).rg*0.1;
    distortedTexCoords = vs_in.texCoords + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
    vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) *
	waveStrength * clamp(waterDepth/distortionDivisor, 0.0, 1.0);
    
    refractTexCoords += totalDistortion;
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
    reflectTexCoords += totalDistortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, 0.001);
    
    vec4 reflectionColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractionColor = texture(refractionTexture, refractTexCoords);

    vec4 normalMapColor = texture(normalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);
    
    vec3 viewVector = normalize(vs_in.toCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 0.5);
    refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);
    
    outColor = mix(reflectionColor, refractionColor, refractiveFactor);
    outColor = mix(outColor, waterColor, colorMixAmount);
    outColor.a = clamp(waterDepth/alphaDivisor, 0.0, 1.0);
}
