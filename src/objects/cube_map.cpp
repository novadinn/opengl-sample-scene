#include "cube_map.h"

#include <math.h>

#include "../file_system.h"
#include "../primitives.h"

namespace {
    const std::string kCubeMapVSFSShaderFilePath = file_system::join("shaders\\skybox.vs");
    const std::string kCubeMapFSFSShaderFilePath = file_system::join("shaders\\skybox.fs");
    const std::vector<std::string> kSkyboxDayTextures = {
	file_system::join("img\\textures\\skybox\\right.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\left.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\top.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\bottom.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\front.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\back.jpg").c_str()
    };
    const std::vector<std::string> kSkyboxNightTextures = {
	file_system::join("img\\textures\\skybox\\nightRight.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\nightLeft.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\nightTop.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\nightBottom.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\nightFront.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\nightBack.jpg").c_str()
    };
    
    const float kRotationSpeed = 1.0f;
    const float kConstant1 = 5000.0f;
    const float kConstant2 = 8000.0f;
    const float kConstant3 = 21000.0f;
    const float kConstant4 = 24000.0f;
}

CubeMap::CubeMap(ResourceLoader& loader) :
    model_(loader.loadToVAO(primitives::cube_positions)),
    shader_(loader.loadVSFSShader(kCubeMapVSFSShaderFilePath.c_str(), kCubeMapFSFSShaderFilePath.c_str())),
    day_texture_(loader.loadCubeMap(kSkyboxDayTextures)),
    night_texture_(loader.loadCubeMap(kSkyboxNightTextures)) {
    
    shader_.bind();
    shader_.setInteger("dayTexture", 0);
    shader_.setInteger("nightTexture", 1);
    shader_.unbind();
}

void CubeMap::draw(glm::mat4& projection, glm::mat4& view) {
    glDepthFunc(GL_LEQUAL);

    shader_.bind();
    model_.bind();
    uint texture1;
    uint texture2;
    float blend_factor;
    if(time_ >= 0 && time_ < kConstant1) {
	texture1 = night_texture_.getId();
	texture2 = night_texture_.getId();
	blend_factor = (time_ - 0)/(kConstant1 - 0);
    } else if(time_ >= kConstant1 && time_ < kConstant2) {
	texture1 = night_texture_.getId();
	texture2 = day_texture_.getId();
	blend_factor = (time_ - kConstant1)/(kConstant2 - kConstant1);
    } else if(time_ >= kConstant2 && time_ < kConstant3) {
	texture1 = day_texture_.getId();
	texture2 = day_texture_.getId();
	blend_factor = (time_ - kConstant2)/(kConstant3 - kConstant2);
    } else {
	texture1 = day_texture_.getId();
	texture2 = night_texture_.getId();
	blend_factor = (time_ - kConstant3)/(kConstant4 - kConstant3);
    }
    Texture3D::activate(0);
    Texture3D::bind(texture1);
    Texture3D::activate(1);
    Texture3D::bind(texture2);
    shader_.setFloat("blendFactor", blend_factor);

    glm::mat4 rotated_view = glm::rotate(view, glm::radians(rotation_), glm::vec3(0.0f, 1.0f, 0.0f));
    shader_.setMatrix4("projection", projection);
    shader_.setMatrix4("view", rotated_view);
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexCount());

    Texture2D::activate(0);
    Texture3D::unbind();
    RawModel::unbind();
    Shader::unbind();

    glDepthFunc(GL_LESS);
}

void CubeMap::update(float delta_time) {
    rotation_ += kRotationSpeed * delta_time;
    
    time_ += delta_time * 1000;
    time_ = fmodf(time_, kConstant4);
}
