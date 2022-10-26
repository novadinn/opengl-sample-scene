#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

struct DirectionalLight {
    DirectionalLight(glm::vec3 dir);

    void draw(Shader shader, glm::vec3& view_pos);

    // TODO: create light_properies strucure (or a generic light class)
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif
