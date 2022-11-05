#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>

struct SpotLight {
    SpotLight() {}
    SpotLight(glm::vec3 position, glm::vec3 direction);

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float constant = 1.0f;
    float linear = 1.0;
    float quadratic = 0.032f;
    float cutoff = glm::cos(glm::radians(12.5f));
    float outer_cutoff = glm::cos(glm::radians(15.0f));
};

#endif
