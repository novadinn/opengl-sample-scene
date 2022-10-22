#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/raw_model.h"
#include "../graphics/shader.h"
#include "../graphics/textures.h"

struct CubeMap {
    CubeMap(RawModel model, Shader shader, Texture3D day_texture, Texture3D night_texture);

    void draw(glm::mat4& projection, glm::mat4& view);
    void update(float delta_time);
    
private:
    RawModel model_;
    Shader shader_;
    Texture3D day_texture_;
    Texture3D night_texture_;

    float rotation_;
    float time_;
};

#endif
