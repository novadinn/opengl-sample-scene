#include "gui_element.h"

#include "../file_system.h"
#include "../primitives.h"

namespace {
    const std::string kGuiVSShaderFilePath = file_system::join("shaders\\gui.vs");
    const std::string kGuiFSShaderFilePath = file_system::join("shaders\\gui.fs");
}

GuiElement::GuiElement(Texture2D texture, ResourceLoader& loader) :
    texture_(texture),
    shader_(loader.loadVSFSShader(kGuiVSShaderFilePath.c_str(), kGuiFSShaderFilePath.c_str())),
    model_(loader.loadToVAO(primitives::plane_positions)),
    size(1.0f, 1.0f) {}

void GuiElement::draw() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader_.bind();
    model_.bind();
    texture_.bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glDisable(GL_DEPTH_TEST); // TODO: test for depth testing and disable if enabled

    shader_.setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexCount());
    
    Texture2D::unbind();
    Shader::unbind();
    RawModel::unbind();
}
