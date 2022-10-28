#include "point_light.h"

namespace point_light {
    const glm::vec3 kDefaultAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
    const glm::vec3 kDefaultDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    const glm::vec3 kDefaultSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
    const float kDefaultConstant = 1.0f;
    const float kDefaultLinear = 0.09f;
    const float kDefaultQuadratic = 0.032f;
}

PointLight::PointLight(glm::vec3 position, int index) :
    position(position), index(index),
    ambient(point_light::kDefaultAmbient),
    diffuse(point_light::kDefaultDiffuse),
    specular(point_light::kDefaultSpecular),
    constant(point_light::kDefaultConstant),
    linear(point_light::kDefaultLinear),
    quadratic(point_light::kDefaultQuadratic) {}
