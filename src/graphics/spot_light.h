#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

struct SpotLight {
    SpotLight(Shader shader);

    void draw(glm::vec3 view_pos);

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutoff;
    float outer_cutoff;
    
private:
    Shader shader_;
};

#endif
