#include "directional_light.h"

namespace directional_light {
    const glm::vec3 kDefaultAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
    const glm::vec3 kDefaultDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    const glm::vec3 kDefaultSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
}

DirectionalLight::DirectionalLight(glm::vec3 dir) :
    direction(dir),
    ambient(directional_light::kDefaultAmbient),
    diffuse(directional_light::kDefaultDiffuse),
    specular(directional_light::kDefaultSpecular) {}
