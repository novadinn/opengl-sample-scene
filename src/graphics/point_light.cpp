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
    position(position), index_(index),
    ambient(point_light::kDefaultAmbient),
    diffuse(point_light::kDefaultDiffuse),
    specular(point_light::kDefaultSpecular),
    constant(point_light::kDefaultConstant),
    linear(point_light::kDefaultLinear),
    quadratic(point_light::kDefaultQuadratic) {}

void PointLight::draw(Shader shader, glm::vec3& view_pos) {
    shader.bind();
    shader.setVector3f("viewPos", view_pos);
    std::string number = std::to_string(index_);
    shader.setVector3f(("pointLights[" + number + "].position").c_str(), position);
    shader.setVector3f(("pointLights[" + number + "].ambient").c_str(), ambient);
    shader.setVector3f(("pointLights[" + number + "].diffuse").c_str(), diffuse);
    shader.setVector3f(("pointLights[" + number + "].specular").c_str(), specular);
    shader.setFloat(("pointLights[" + number + "].constant").c_str(), constant);
    shader.setFloat(("pointLights[" + number + "].linear").c_str(), linear);
    shader.setFloat(("pointLights[" + number + "].quadratic").c_str(), quadratic);
    Shader::unbind();
}
