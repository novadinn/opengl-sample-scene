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
