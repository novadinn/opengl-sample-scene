#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>

struct DirectionalLight {
    DirectionalLight(glm::vec3 dir);

    glm::vec3 direction;
    glm::vec3 ambient = glm::vec3(0.005f, 0.005f, 0.005f);
    glm::vec3 diffuse = glm::vec3(0.04f, 0.04f, 0.04f);
    glm::vec3 specular = glm::vec3(0.05f, 0.05f, 0.05f);
};

#endif
