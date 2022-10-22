#include "directional_light.h"

namespace directional_light {
    const glm::vec3 kDefaultAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
    const glm::vec3 kDefaultDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    const glm::vec3 kDefaultSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
}

DirectionalLight::DirectionalLight(Shader shader) :
    shader_(shader),
    ambient(directional_light::kDefaultAmbient),
    diffuse(directional_light::kDefaultDiffuse),
    specular(directional_light::kDefaultSpecular) {}

DirectionalLight::DirectionalLight(Shader shader, glm::vec3 dir) :
    shader_(shader), direction(dir),
    ambient(directional_light::kDefaultAmbient),
    diffuse(directional_light::kDefaultDiffuse),
    specular(directional_light::kDefaultSpecular) {}

void DirectionalLight::draw(glm::vec3 view_pos) {
    shader_.bind();
    shader_.setVector3f("viewPos", view_pos);
    shader_.setVector3f("dirLight.direction", direction);
    shader_.setVector3f("dirLight.ambient", ambient);
    shader_.setVector3f("dirLight.diffuse", diffuse);
    shader_.setVector3f("dirLight.specular", specular);
    Shader::unbind();
}
