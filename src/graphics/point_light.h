#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>

#include "shader.h"

struct PointLight {
    PointLight(glm::vec3 position, int index);

    void draw(Shader shader, glm::vec3& view_pos);
    
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;

private:
    int index_;
};

#endif
