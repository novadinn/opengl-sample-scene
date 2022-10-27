#include "water.h"

#include <GLFW/glfw3.h>

#include "../file_system.h"
#include "../primitives.h"

namespace {
    const std::string kWaterVSShaderFilePath = file_system::join("shaders\\water.vs");
    const std::string kWaterFSShaderFilePath = file_system::join("shaders\\water.fs");
    
    const std::string kTextureFilePath = file_system::join("img\\textures\\container2.png");

    const int kReflectionWidth = 320;
    const int kReflectionHeight = 180;	
    const int kRefractionWidth = 1280;
    const int kRefractionHeight = 720;
}

Water::Water(ResourceLoader& loader) :
    GameObject(loader.loadToVAO(
		   primitives::plane_positions, primitives::plane_normals, primitives::plane_tex_coords),
	       loader.loadVSFSShader(kWaterVSShaderFilePath.c_str(), kWaterFSShaderFilePath.c_str())) {

    reflection_frame_buffer_ = loader.createFrameBuffer();
    reflection_texture_ = loader.createTextureAttachment(kReflectionWidth, kReflectionHeight);
    reflection_depth_buffer_ = loader.createDepthBufferAttachment(kReflectionWidth, kReflectionHeight);;

    refraction_frame_buffer_ = loader.createFrameBuffer();
    refraction_texture_ = loader.createTextureAttachment(kRefractionWidth, kRefractionHeight);
    refraction_depth_texture_ = loader.createDepthTextureAttachment(kRefractionWidth, kRefractionHeight);;
}

void Water::bindReflectionFrameBuffer() {
    reflection_frame_buffer_.bind(kReflectionWidth, kReflectionHeight);
}

void Water::bindRefractionFrameBuffer() {
    refraction_frame_buffer_.bind(kRefractionWidth, kRefractionHeight);
}

void Water::unbindCurrentFrameBuffer() {
    FrameBuffer::unbind();
}

void Water::draw(glm::mat4& projection, glm::mat4& view) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.5f * size.z));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z));
    model = glm::scale(model, size);

    shader_.bind();
    model_.bind();
    RawModel::enableAttribute(0);
    RawModel::enableAttribute(1);
    RawModel::enableAttribute(2);
    
    shader_.setMatrix4("projection", projection);
    shader_.setMatrix4("view", view);
    shader_.setMatrix4("model", model);
    
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexCount());

    RawModel::disableAttribute(0);
    RawModel::disableAttribute(1);
    RawModel::disableAttribute(2);
    RawModel::unbind();
    Shader::unbind();
}

Texture2D Water::getReflectionTexture() const {
    return reflection_texture_;
}

Texture2D Water::getRefractionTexture() const {
    return refraction_texture_;
}
