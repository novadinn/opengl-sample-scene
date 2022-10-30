#include "cube_map.h"

#include <math.h>

#include "../file_system.h"
#include "../primitives.h"

namespace {
    const std::string kCubeMapVSFSShaderFilePath = file_system::join("shaders\\skybox_vert.glsl");
    const std::string kCubeMapFSFSShaderFilePath = file_system::join("shaders\\skybox_frag.glsl");
    const std::vector<std::string> kSkyboxDayTextures = {
	file_system::join("img\\textures\\skybox\\right.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\left.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\top.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\bottom.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\front.jpg").c_str(),
	file_system::join("img\\textures\\skybox\\back.jpg").c_str()
    };
    
    const float kRotationSpeed = 1.0f;
    const float kConstant1 = 5000.0f;
    const float kConstant2 = 8000.0f;
    const float kConstant3 = 21000.0f;
    const float kConstant4 = 24000.0f;
}

CubeMap::CubeMap(ResourceLoader& loader) :
    model_(loader.loadToVAO(primitives::cube_positions, 3)),
    shader_(loader.loadVSFSShader(kCubeMapVSFSShaderFilePath.c_str(), kCubeMapFSFSShaderFilePath.c_str())),
    day_texture_(loader.loadCubeMap(kSkyboxDayTextures)) {
    
    shader_.bind();
    shader_.setInteger("dayTexture", 0);
    shader_.unbind();
}

void CubeMap::draw(glm::mat4& projection, glm::mat4& view) {
    glDepthFunc(GL_LEQUAL);

    shader_.bind();
    Texture3D::activate(0);
    day_texture_.bind();
    model_.bind();
    RawModel::enableAttribute(0);

    glm::mat4 rotated_view = glm::rotate(view, glm::radians(rotation_), glm::vec3(0.0f, 1.0f, 0.0f));
    shader_.setMatrix4("projection", projection);
    shader_.setMatrix4("view", rotated_view);
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexCount());

    RawModel::disableAttribute(0);
    RawModel::unbind();
    Texture2D::deactivate();
    Texture3D::unbind();
    Shader::unbind();

    glDepthFunc(GL_LESS);
}

void CubeMap::update(float delta_time) {
    rotation_ += kRotationSpeed * delta_time;
}
