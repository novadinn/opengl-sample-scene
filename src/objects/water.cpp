#include "water.h"

#include <GLFW/glfw3.h>

#include "../file_system.h"
#include "../primitives.h"
#include "../display.h"

namespace {
    const std::string kWaterVSShaderFilePath = file_system::join("shaders\\water_vert.glsl");
    const std::string kWaterFSShaderFilePath = file_system::join("shaders\\water_frag.glsl");
    
    const int kReflectionWidth = 320;
    const int kReflectionHeight = 180;	
    const int kRefractionWidth = 1280;
    const int kRefractionHeight = 720;
    
    const std::string kDuDvMapFilePath = file_system::join("img\\textures\\water\\water_dudv.png");
    const std::string kNormalMapFilePath = file_system::join("img\\textures\\water\\water_normal_map.png");
    const float kMoveSpeed = 0.03f;
}

Water::Water(ResourceLoader& loader) :
    GameObject(loader.loadVSFSShader(kWaterVSShaderFilePath.c_str(), kWaterFSShaderFilePath.c_str())),
    model_(loader.loadToVAO(primitives::plane_positions, primitives::plane_normals,
			    primitives::plane_tex_coords)),
    dudv_map_(loader.loadTexture(kDuDvMapFilePath.c_str())),
    normal_map_(loader.loadTexture(kNormalMapFilePath.c_str())) {

    reflection_frame_buffer_ = loader.createFrameBuffer();
    reflection_texture_ = loader.createTextureAttachment(kReflectionWidth, kReflectionHeight);
    reflection_depth_buffer_ = loader.createDepthBufferAttachment(kReflectionWidth, kReflectionHeight);;

    refraction_frame_buffer_ = loader.createFrameBuffer();
    refraction_texture_ = loader.createTextureAttachment(kRefractionWidth, kRefractionHeight);
    refraction_depth_texture_ = loader.createDepthTextureAttachment(kRefractionWidth, kRefractionHeight);;

    shader_.bind();
    shader_.setInteger("reflectionTexture", 0);
    shader_.setInteger("refractionTexture", 1);
    shader_.setInteger("dudvMap", 2);
    shader_.setInteger("normalMap", 3);
    shader_.setInteger("depthMap", 4);
    Shader::unbind();
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

void Water::draw(glm::mat4& projection, glm::mat4& view, glm::vec3 camera_position) {

    prepareDrawing();
    setMVP(projection, view);
    draw(camera_position);
    endDrawing();
}

void Water::update(float delta_time) {
    move_factor_ += kMoveSpeed * delta_time;
    move_factor_ = fmodf(move_factor_, 1);
}

Texture2D Water::getReflectionTexture() const {
    return reflection_texture_;
}

Texture2D Water::getRefractionTexture() const {
    return refraction_texture_;
}


void Water::prepareDrawing() {
    shader_.bind();
    Texture2D::activate(0);
    reflection_texture_.bind();
    Texture2D::activate(1);
    refraction_texture_.bind();
    Texture2D::activate(2);
    dudv_map_.bind();
    Texture2D::activate(3);
    normal_map_.bind();
    Texture2D::activate(4);
    refraction_depth_texture_.bind();
    model_.bind();
    RawModel::enableAttribute(0);
    RawModel::enableAttribute(1);
    RawModel::enableAttribute(2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Water::draw(glm::vec3 camera_position) {
    shader_.setVector3f("cameraPosition", camera_position);
    shader_.setFloat("moveFactor", move_factor_);
    shader_.setFloat("waveStrength", wave_strength);
    shader_.setFloat("shineDamper", shine_damper);
    shader_.setFloat("reflectivity", reflectivity);
    shader_.setFloat("near", Display::near_plane);
    shader_.setFloat("far", Display::far_plane);
    
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexCount());
}

void Water::endDrawing() {
    glDisable(GL_BLEND);
    
    RawModel::disableAttribute(0);
    RawModel::disableAttribute(1);
    RawModel::disableAttribute(2);
    RawModel::unbind();
    Texture2D::deactivate();
    Texture2D::unbind();
    Shader::unbind();
}
