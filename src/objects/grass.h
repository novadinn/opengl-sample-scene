#ifndef GRASS_H
#define GRASS_H

#include "model.h"

// NOTE: possibly rename to terrain
struct Grass : public Model {
    Grass(ResourceLoader& loader);

    void update(float delta_time);
    void draw(glm::mat4& projection, glm::mat4& view,
	      glm::vec3& view_pos, DirectionalLight& dir_light,
	      SpotLight& spot_light, PointLight& point_light) override;

    float bend_rotation_random = 0.2f; // NOTE: Range from 0 to 1
    float blade_width = 0.5f;
    float blade_width_random = 0.2f;
    float blade_height = 5.0f;
    float blade_height_random = 3.0f;

    glm::vec2 wind_frequency = glm::vec2(0.05f, 0.05f);
    glm::vec2 wind_distortion_map_tiling = glm::vec2(1.0f, 1.0f);
    glm::vec2 wind_distortion_map_offset = glm::vec2(0.0f, 0.0f);
    float wind_strength = 1.0f;
    float time;

private:
    Texture2D distortion_map_;
};

#endif
