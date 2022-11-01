#include "grass.h"

#include "../file_system.h"

namespace {
    const std::string kGrassVSShaderFilePath = file_system::join("shaders\\grass_vert.glsl").c_str();
    const std::string kGrassGSShaderFilePath = file_system::join("shaders\\grass_geom.glsl").c_str();
    const std::string kGrassFSShaderFilePath = file_system::join("shaders\\grass_frag.glsl").c_str();

    const std::string kGrassPlaneFilePath = file_system::join("objects\\grass\\grass_plane.obj");
    const std::string kWindDistortionFilePath = file_system::join("img\\textures\\grass\\wind_distortion.png");
}

Grass::Grass(ResourceLoader& loader) :
    Model(loader, loader.loadVSGSFSShader(kGrassVSShaderFilePath.c_str(),
					  kGrassGSShaderFilePath.c_str(),
					  kGrassFSShaderFilePath.c_str()),
	  kGrassPlaneFilePath),
    distortion_map_(loader.loadTexture(kWindDistortionFilePath.c_str())) {
    
    shader_.bind();
    shader_.setInteger("windDistortionMap", 0);
    Shader::unbind();
}

void Grass::update(float delta_time) {
    time += delta_time;
}

void Grass::draw(glm::mat4& projection, glm::mat4& view,
		 glm::vec3& view_pos, DirectionalLight& dir_light,
		 SpotLight& spot_light, PointLight& point_light) {

    shader_.bind();
    Texture2D::activate(0);
    distortion_map_.bind();
    
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

    shader_.setFloat("bendRotationRandom", bend_rotation_random);
    shader_.setFloat("bladeWidth", blade_width);
    shader_.setFloat("bladeWidthRandom", blade_width_random);
    shader_.setFloat("bladeHeight", blade_height);
    shader_.setFloat("bladeHeightRandom", blade_height_random);

    shader_.setVector2f("windFrequency", wind_frequency);
    shader_.setVector2f("windDistortionMapTiling", wind_distortion_map_tiling);
    shader_.setVector2f("windDistortionMapOffset", wind_distortion_map_offset);
    shader_.setFloat("windStrength", wind_strength);
    shader_.setFloat("time", time);
    
    for(int i = 0; i < meshes_.size(); ++i)
	meshes_[i].draw();

    Texture2D::deactivate();
    Texture2D::unbind();
    Shader::unbind();
}
