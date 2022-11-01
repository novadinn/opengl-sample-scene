#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 7) out; // blade_segment_count * 2 + 1

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_in[];

out GS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} gs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform float bendRotationRandom;
uniform float bladeWidth;
uniform float bladeWidthRandom;
uniform float bladeHeight;
uniform float bladeHeightRandom;

uniform sampler2D windDistortionMap;
uniform vec2 windFrequency;
uniform vec2 windDistortionMapTiling;
uniform vec2 windDistortionMapOffset;
uniform float windStrength;
uniform float time;

#define PI 3.1415926538

vec4 objectToClipPos(vec4 pos);

mat3 angleAxisMat3(float angle, vec3 axis);

uint hash(uint x);
uint hash(uvec2 v);
uint hash(uvec3 v);
uint hash(uvec4 v);
float floatConstruct(uint m);
float random(float x);
float random(vec2 v);
float random(vec3 v);
float random(vec4 v);
mat3 mul(mat3 l, mat3 r) { // TODO: remove this
    return l * r;
}

// geometryOutput generateGrassVertex(vec3 vertexPosition, float width, float height, vec2 uv, mat3 transformMatrix) {
//     vec3 tangentPoint = vec3(width, 0, height);

//     vec3 localPosition = vertexPosition + mul(transformMatrix, tangentPoint);
//     return VertexOutput(localPosition, uv);
// }

void main() {
    vec4 startPos = gl_in[0].gl_Position;
    mat3 facingRotationMatrix = angleAxisMat3(random(startPos) * (2 * PI), vec3(0.0, 0.0, 1.0));
    mat3 bendRotationMatrix = angleAxisMat3(random(startPos.zzx) * bendRotationRandom * PI * 0.5, vec3(-1, 0, 0));

    vec2 uv = startPos.xz * windDistortionMapTiling + windDistortionMapOffset + windFrequency * time;
    vec2 windSample = (texture(windDistortionMap, uv).xy * 2 - 1) * windStrength;
    vec3 wind = normalize(vec3(windSample.x, windSample.y, 0.0));
    mat3 windRotation = angleAxisMat3(PI * windSample.x, wind);
    
    float height = (random(startPos.zyx) * 2.0 - 1.0) * bladeHeightRandom + bladeHeight;
    float width = (random(startPos.xzy) * 2.0 - 1.0) * bladeWidthRandom + bladeWidth;
    vec3 positions[] = vec3[](vec3(width, 0.0, 0.0), vec3(-width, 0.0, 0.0), vec3(0.0, 0.0, height));
    vec2 texCoords[] = vec2[](vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.5, 1.0));

    int i = 0;
    gs_out.Normal = vs_in[i].Normal;
    gs_out.FragPos = vs_in[i].FragPos;
    gs_out.TexCoords = texCoords[i];
    gs_out.TBN = vs_in[i].TBN;
	
    mat3 transformationMatrixFacing = mul(vs_in[i].TBN, facingRotationMatrix);
    vec3 currentPos = transformationMatrixFacing * positions[i];
    gl_Position = objectToClipPos(vec4(currentPos, 0.0) + startPos);
    EmitVertex();

    i = 1;
    gs_out.Normal = vs_in[i].Normal;
    gs_out.FragPos = vs_in[i].FragPos;
    gs_out.TexCoords = texCoords[i];
    gs_out.TBN = vs_in[i].TBN;
	
    transformationMatrixFacing = mul(vs_in[i].TBN, facingRotationMatrix);
    currentPos = transformationMatrixFacing * positions[i];
    gl_Position = objectToClipPos(vec4(currentPos, 0.0) + startPos);
    EmitVertex();

    i = 2;
    gs_out.Normal = vs_in[i].Normal;
    gs_out.FragPos = vs_in[i].FragPos;
    gs_out.TexCoords = texCoords[i];
    gs_out.TBN = vs_in[i].TBN;
	
    mat3 transformationMatrix = mul(mul(mul(vs_in[i].TBN, windRotation),
					facingRotationMatrix), bendRotationMatrix);
    currentPos = transformationMatrix * positions[i];
    gl_Position = objectToClipPos(vec4(currentPos, 0.0) + startPos);
    EmitVertex();
    
    EndPrimitive();
}

vec4 objectToClipPos(vec4 pos) {
    mat4 mvp = projection * view * model;
    return mvp * pos;
}

mat3 angleAxisMat3(float angle, vec3 axis) {
    float c = cos(angle);
    float s = sin(angle);

    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return mat3(
        t * x * x + c,      t * x * y - s * z,  t * x * z + s * y,
        t * x * y + s * z,  t * y * y + c,      t * y * z - s * x,
        t * x * z - s * y,  t * y * z + s * x,  t * z * z + c
    );
}

uint hash(uint x) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}
uint hash(uvec2 v) {
    return hash(v.x ^ hash(v.y));
}
uint hash(uvec3 v) {
    return hash(v.x ^ hash(v.y) ^ hash(v.z));
}
uint hash(uvec4 v) {
    return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w));
}

float floatConstruct(uint m) {
    const uint ieeeMantissa = 0x007FFFFFu;
    const uint ieeeOne = 0x3F800000u;

    m &= ieeeMantissa;
    m |= ieeeOne;

    float  f = uintBitsToFloat(m);
    return f - 1.0;
}

float random(float x) {
    return floatConstruct(hash(floatBitsToUint(x)));
}
float random(vec2 v) {
    return floatConstruct(hash(floatBitsToUint(v)));
}
float random(vec3 v) {
    return floatConstruct(hash(floatBitsToUint(v)));
}
float random(vec4 v) {
    return floatConstruct(hash(floatBitsToUint(v)));
}
