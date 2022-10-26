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

SpotLight::SpotLight() :
    ambient(spot_light::kDefaultAmbient),
    diffuse(spot_light::kDefaultDiffuse),
    specular(spot_light::kDefaultSpecular),
    constant(spot_light::kDefaultConstant),
    linear(spot_light::kDefaultLinear),
    quadratic(spot_light::kDefaultQuadratic),
    cutoff(spot_light::kDefaultCutoff),
    outer_cutoff(spot_light::kDefaultOuterCutoff) {}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction) :
    position(position), direction(direction),
    ambient(spot_light::kDefaultAmbient),
    diffuse(spot_light::kDefaultDiffuse),
    specular(spot_light::kDefaultSpecular),
    constant(spot_light::kDefaultConstant),
    linear(spot_light::kDefaultLinear),
    quadratic(spot_light::kDefaultQuadratic),
    cutoff(spot_light::kDefaultCutoff),
    outer_cutoff(spot_light::kDefaultOuterCutoff) {}

void SpotLight::draw(Shader shader, glm::vec3& view_pos) {
    shader.bind();
    shader.setVector3f("viewPos", view_pos);
    shader.setVector3f("spotLight.position", position);
    shader.setVector3f("spotLight.direction", direction);
    shader.setVector3f("spotLight.ambient", ambient);
    shader.setVector3f("spotLight.diffuse", diffuse);
    shader.setVector3f("spotLight.specular", specular);
    shader.setFloat("spotLight.constant", constant);
    shader.setFloat("spotLight.linear", linear);
    shader.setFloat("spotLight.quadratic", quadratic);
    shader.setFloat("spotLight.cutOff", cutoff);
    shader.setFloat("spotLight.outerCutOff", outer_cutoff);
    Shader::unbind();
}
