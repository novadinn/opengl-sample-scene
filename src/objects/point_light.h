#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>

struct PointLight {
    PointLight(glm::vec3 position, int index);

    int index;
    
    glm::vec3 position;
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float constant = 1.0f;
    float linear = 1.0f;
    float quadratic = 0.032f;
};

#endif
