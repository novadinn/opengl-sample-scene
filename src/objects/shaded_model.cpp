#include "shaded_model.h"

ShadedModel::ShadedModel(ResourceLoader& loader, Shader shader, std::string const &path) :
    GameObject(shader), model_(loader, shader, path) {}

void ShadedModel::draw(glm::mat4& projection, glm::mat4& view) {
    prepareDrawing();
    setMVP(projection, view);
    draw();
    endDrawing();
}
    
void ShadedModel::prepareDrawing() {
    shader_.bind();
}

void ShadedModel::draw() {
    model_.draw();
}

void ShadedModel::endDrawing() {
    Shader::unbind();
}
