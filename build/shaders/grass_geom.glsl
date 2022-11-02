#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 7) out;

in VS_OUT {
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_in[];

out GS_OUT {
    vec2 TexCoords;
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

uniform float bladeForward;
uniform float bladeCurvatureAmount;
// TODO: add lighting calculations

#define PI 3.1415926538

vec4 objectToClipPos(vec4 pos);
mat3 angleAxisMat3(float angle, vec3 axis);

vec4 generateGrassVertex(vec4 vertexPosition, float width, float height, float forward, mat3 transformMatrix);

uint hash(uint x);
uint hash(uvec2 v);
uint hash(uvec3 v);
uint hash(uvec4 v);
float floatConstruct(uint m);
float random(float x);
float random(vec2 v);
float random(vec3 v);
float random(vec4 v);

void main() {
    vec4 startPos = gl_in[0].gl_Position;
    mat3 facingRotationMatrix = angleAxisMat3(random(startPos) * (2 * PI), vec3(0.0, 0.0, 1.0));
    mat3 bendRotationMatrix = angleAxisMat3(random(startPos.zzx) * bendRotationRandom * PI * 0.5, vec3(-1, 0, 0));
    mat3 TBN = vs_in[0].TBN;
    
    vec2 uv = startPos.xz * windDistortionMapTiling + windDistortionMapOffset + windFrequency * time;
    vec2 windSample = (texture(windDistortionMap, uv).xy * 2 - 1) * windStrength;
    vec3 wind = normalize(vec3(windSample.x, windSample.y, 0.0));
    mat3 windRotation = angleAxisMat3(PI * windSample.x, wind);

    mat3 transformationMatrixFacing = TBN * facingRotationMatrix;
    mat3 transformationMatrix = TBN * windRotation * facingRotationMatrix * bendRotationMatrix;
    
    float height = (random(startPos.zyx) * 2.0 - 1.0) * bladeHeightRandom + bladeHeight;
    float width = (random(startPos.xzy) * 2.0 - 1.0) * bladeWidthRandom + bladeWidth;
    float forward = random(startPos.yyz) * bladeForward;

    for(int i = 0; i < 3; ++i) {
	float t = float(i) / 3.0;
	float segmentHeight = height * t;
	float segmentWidth = width * (1 - t);
	float segmentForward = pow(t, bladeCurvatureAmount) * forward;
	mat3 transformMatrix = i == 0 ? transformationMatrixFacing : transformationMatrix;
	
	gs_out.TexCoords = vec2(0.0, t);
	gl_Position = generateGrassVertex(startPos, segmentWidth, segmentHeight, segmentForward, transformMatrix);
	EmitVertex();

	gs_out.TexCoords = vec2(1.0, t);
	gl_Position = generateGrassVertex(startPos, -segmentWidth, segmentHeight, segmentForward, transformMatrix);
	EmitVertex();
    }

    gs_out.TexCoords = vec2(0.5, 1.0);
    gl_Position = generateGrassVertex(startPos, 0.0, height, forward, transformationMatrix);
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

vec4 generateGrassVertex(vec4 vertexPosition, float width, float height, float forward, mat3 transformMatrix) {
    vec3 tangentPoint = vec3(width, forward, height);

    vec3 localPosition = vertexPosition.xyz + (transformMatrix * tangentPoint);
    return objectToClipPos(vec4(localPosition, vertexPosition.w));
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
