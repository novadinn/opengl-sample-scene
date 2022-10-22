#include "spot_light.h"

namespace spot_light {
    const glm::vec3 kDefaultAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
    const glm::vec3 kDefaultDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    const glm::vec3 kDefaultSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
    const float kDefaultConstant = 1.0f;
    const float kDefaultLinear = 0.09f;
    const float kDefaultQuadratic = 0.032f;
    const float kDefaultCutoff = glm::cos(glm::radians(12.5f));
    const float kDefaultOuterCutoff = glm::cos(glm::radians(15.0f));
}

SpotLight::SpotLight(Shader shader) :
    shader_(shader),
    ambient(spot_light::kDefaultAmbient),
    diffuse(spot_light::kDefaultDiffuse),
    specular(spot_light::kDefaultSpecular),
    constant(spot_light::kDefaultConstant),
    linear(spot_light::kDefaultLinear),
    quadratic(spot_light::kDefaultQuadratic),
    cutoff(spot_light::kDefaultCutoff),
    outer_cutoff(spot_light::kDefaultOuterCutoff) {}

void SpotLight::draw(glm::vec3 view_pos) {
    shader_.bind();
    shader_.setVector3f("viewPos", view_pos);
    shader_.setVector3f("spotLight.position", position);
    shader_.setVector3f("spotLight.direction", direction);
    shader_.setVector3f("spotLight.ambient", ambient);
    shader_.setVector3f("spotLight.diffuse", diffuse);
    shader_.setVector3f("spotLight.specular", specular);
    shader_.setFloat("spotLight.constant", constant);
    shader_.setFloat("spotLight.linear", linear);
    shader_.setFloat("spotLight.quadratic", quadratic);
    shader_.setFloat("spotLight.cutOff", cutoff);
    shader_.setFloat("spotLight.outerCutOff", outer_cutoff);
    Shader::unbind();
}
