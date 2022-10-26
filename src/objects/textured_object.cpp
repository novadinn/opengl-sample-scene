#include "textured_object.h"

TexturedObject::TexturedObject(RawModel model, Shader shader, Texture2D diffuse, Texture2D specular) :
    GameObject(model, shader), diffuse_(diffuse), specular_(specular) {
    
    shader_.bind();
    shader_.setInteger("material.diffuse", 0);
    shader_.setInteger("material.specular", 1);
    shader_.setFloat("material.shininess", 32.0f);
    Shader::unbind();
}

void TexturedObject::draw(glm::mat4& projection, glm::mat4& view) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
    model = glm::scale(model, size);

    Texture2D::activate(0);
    diffuse_.bind();
    Texture2D::activate(1);
    specular_.bind();
    shader_.bind();
    model_.bind();

    shader_.setMatrix4("projection", projection);
    shader_.setMatrix4("view", view);
    shader_.setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexCount());
    
    Texture2D::activate(0);
    Texture2D::unbind();
    RawModel::unbind();
    Shader::unbind();
}
