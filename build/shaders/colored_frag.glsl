#version 330 core

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec3 color;
} vs_in;

out vec4 outColor;

#define MAX_POINT_LIGHTS 1

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform float shininess;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(vs_in.normal);
    vec3 viewDir = normalize(viewPos - vs_in.fragPos);
 
    vec3 result = calcDirLight(dirLight, norm, viewDir);
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
        result += calcPointLight(pointLights[i], norm, vs_in.fragPos, viewDir);
    result += calcSpotLight(spotLight, norm, vs_in.fragPos, viewDir);    
    
    outColor = vec4(vs_in.color, 1.0) + vec4(result, 0.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float distance = length(light.position - fragPos);
    
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}

// vec3 ADSLightModel(in vec3 myNormal, in vec3 myPosition ) {
//     const vec3 myLightPosition = vec3(1.0, 0.5, 0.0);
//     const vec3 myLightAmbient = vec3(0.2, 0.2, 0.2);
//     const vec3 myLightDiffuse = vec3(1.0, 1.0, 1.0);
//     const vec3 myLightSpecular = vec3(1.0, 1.0, 1.0);
//     const vec3 myMaterialAmbient = vec3(1.0, 0.5, 0.0);
//     const vec3 myMaterialDiffuse = vec3(1.0, 0.5, 0.0);
//     const vec3 myMaterialSpecular = vec3(0.6, 0.6, 0.6);
//     const float myMaterialShininess = 80.0;

//     vec3 norm = normalize(myNormal);
//     vec3 lightv = normalize(myLightPosition - myPosition);
//     vec3 viewv = normalize(vec3(0.,0.,0.) - myPosition);
//     vec3 refl = reflect(vec3(0.,0.,0.) - lightv, norm);

//     vec3 ambient = myMaterialAmbient*myLightAmbient;

//     vec3 diffuse = max(0.0, dot(lightv, norm)) * myMaterialDiffuse * myLightDiffuse;

//     vec3 specular = vec3( 0.0, 0.0, 0.0 );
//     if(dot(lightv, viewv) > 0.0) {
// 	specular = pow(max(0.0, dot(viewv, refl)), myMaterialShininess) * myMaterialSpecular * myLightSpecular;
//     }
    
//     return clamp(ambient + diffuse + specular, 0.0, 1.0);
// }
