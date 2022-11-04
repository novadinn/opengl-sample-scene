#include "grass.h"

#include "../file_system.h"

namespace {
    const std::string kGrassVSShaderFilePath = file_system::join("shaders\\grass_vert.glsl").c_str();
    const std::string kGrassGSShaderFilePath = file_system::join("shaders\\grass_geom.glsl").c_str();
    const std::string kGrassFSShaderFilePath = file_system::join("shaders\\grass_frag.glsl").c_str();

    const std::string kGrassPlaneFilePath = file_system::join("objects\\grass\\grass_plane.obj");
    const std::string kWindDistortionFilePath = file_system::join("img\\textures\\grass\\wind_distortion.png");

    const float kBendRotationMin = 0.0f;
    const float kBendRotationMax = 1.0f;
    const float kBladeCurvatureAmountMin = 1.0f;
    const float kBladeCurvatureAmountMax = 4.0f;
}

Grass::Grass(ResourceLoader& loader) :
    ShadedModel(loader,
	  loader.loadVSGSFSShader(kGrassVSShaderFilePath.c_str(),
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

void Grass::draw(glm::mat4& projection, glm::mat4& view) {

    prepareDrawing();
    setMVP(projection, view);
    draw();
    endDrawing();    
}

void Grass::setBendRotationRandom(float val) {
    bend_rotation_random_ = glm::clamp(bend_rotation_random_+val, kBendRotationMin, kBendRotationMax);
}

float Grass::getBendRotationRandom() const {
    return bend_rotation_random_;
}

void Grass::setBladeCurvatureAmount(float val) {
    blade_curvature_amount_ = glm::clamp(blade_curvature_amount_+val, kBladeCurvatureAmountMin,
					kBladeCurvatureAmountMax);
}

float Grass::getBladeCurvatureAmount() const {
    return blade_curvature_amount_;
}

void Grass::prepareDrawing() {
    ShadedModel::prepareDrawing();
    
    Texture2D::activate(0);
    distortion_map_.bind();
}

void Grass::draw() {
    shader_.setFloat("bendRotationRandom", bend_rotation_random_);
    shader_.setFloat("bladeWidth", blade_width);
    shader_.setFloat("bladeWidthRandom", blade_width_random);
    shader_.setFloat("bladeHeight", blade_height);
    shader_.setFloat("bladeHeightRandom", blade_height_random);

    shader_.setVector2f("windFrequency", wind_frequency);
    shader_.setVector2f("windDistortionMapTiling", wind_distortion_map_tiling);
    shader_.setVector2f("windDistortionMapOffset", wind_distortion_map_offset);
    shader_.setFloat("windStrength", wind_strength);
    shader_.setFloat("time", time);

    shader_.setFloat("bladeForward", blade_forward);
    shader_.setFloat("bladeCurvatureAmount", blade_curvature_amount_);
    
    ShadedModel::draw();
}

void Grass::endDrawing() {
    Texture2D::deactivate();
    Texture2D::unbind();
    
    ShadedModel::endDrawing();
}
