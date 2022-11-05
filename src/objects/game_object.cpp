#include "game_object.h"

GameObject::GameObject(Shader shader) :
    shader_(shader), 
    position(glm::vec3(0.0f)), size(glm::vec3(1.0f)), rotation(glm::vec3(0.0f)) {}

void GameObject::draw(glm::mat4& projection, glm::mat4& view) {
    prepareDrawing();
    setMVP(projection, view);
    draw();
    endDrawing();
}

void GameObject::useDirectionalLight(glm::vec3& view_pos, DirectionalLight& light) {
    shader_.bind();
    shader_.setVector3f("viewPos", view_pos);
    shader_.setVector3f("dirLight.direction", light.direction);
    shader_.setVector3f("dirLight.ambient", light.ambient);
    shader_.setVector3f("dirLight.diffuse", light.diffuse);
    shader_.setVector3f("dirLight.specular", light.specular);
    Shader::unbind();
}

void GameObject::usePointLight(glm::vec3& view_pos, PointLight& light) {
    shader_.bind();
    std::string number = std::to_string(light.index);
    shader_.setVector3f("viewPos", view_pos);
    shader_.setVector3f(("pointLights[" + number + "].position").c_str(), light.position);
    shader_.setVector3f(("pointLights[" + number + "].ambient").c_str(), light.ambient);
    shader_.setVector3f(("pointLights[" + number + "].diffuse").c_str(), light.diffuse);
    shader_.setVector3f(("pointLights[" + number + "].specular").c_str(), light.specular);
    shader_.setFloat(("pointLights[" + number + "].constant").c_str(), light.constant);
    shader_.setFloat(("pointLights[" + number + "].linear").c_str(), light.linear);
    shader_.setFloat(("pointLights[" + number + "].quadratic").c_str(), light.quadratic);
    Shader::unbind();
}

void GameObject::useSpotLight(glm::vec3& view_pos, SpotLight& light) {
    shader_.bind();
    shader_.setVector3f("viewPos", view_pos);
    shader_.setVector3f("spotLight.position", light.position);
    shader_.setVector3f("spotLight.direction", light.direction);
    shader_.setVector3f("spotLight.ambient", light.ambient);
    shader_.setVector3f("spotLight.diffuse", light.diffuse);
    shader_.setVector3f("spotLight.specular", light.specular);
    shader_.setFloat("spotLight.constant", light.constant);
    shader_.setFloat("spotLight.linear", light.linear);
    shader_.setFloat("spotLight.quadratic", light.quadratic);
    shader_.setFloat("spotLight.cutOff", light.cutoff);
    shader_.setFloat("spotLight.outerCutOff", light.outer_cutoff);
    Shader::unbind();
}

void GameObject::prepareDrawing() {
    shader_.bind();
}

void GameObject::setMVP(glm::mat4& projection, glm::mat4& view) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
    model = glm::scale(model, size);

    shader_.setMatrix4("projection", projection);
    shader_.setMatrix4("view", view);
    shader_.setMatrix4("model", model);
}

void GameObject::draw() {}

void GameObject::endDrawing() {
    Shader::unbind();
}
