#ifndef GRASS_H
#define GRASS_H

#include "../graphics/model.h"
#include "game_object.h"

// NOTE: possibly rename to terrain
struct Grass : public GameObject {
    Grass(ResourceLoader& loader);

    void update(float delta_time);
    void draw(glm::mat4& projection, glm::mat4& view);

    void setBendRotationRandom(float val);
    float getBendRotationRandom() const;
    void setBladeCurvatureAmount(float val);
    float getBladeCurvatureAmount() const;
    
    float blade_width = 0.05f;
    float blade_width_random = 0.02f;
    float blade_height = 0.5f;
    float blade_height_random = 0.3f;

    glm::vec2 wind_frequency = glm::vec2(0.01f, 0.01f);
    glm::vec2 wind_distortion_map_tiling = glm::vec2(1.0f, 1.0f);
    glm::vec2 wind_distortion_map_offset = glm::vec2(0.0f, 0.0f);
    float wind_strength = 1.0f;
    float time;

    float blade_forward = 0.38f;
    
    glm::vec3 plane_color = glm::vec3(0.2f, 0.7f, 0.01);

private:
    void prepareDrawing() override;
    void draw();
    void endDrawing() override;
    
    Model grass_model_;
    Shader plane_shader_; // TODO: connect those 2 in 1 class
    Model plane_model_;
    Texture2D distortion_map_;

    float bend_rotation_random_ = 0.2f;
    float blade_curvature_amount_ = 2.0f;
};

#endif
