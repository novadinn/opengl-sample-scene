#include "cube_map.h"

#include <math.h>

namespace {
    const float kRotationSpeed = 1.0f;
    const float kConstant1 = 5000.0f;
    const float kConstant2 = 8000.0f;
    const float kConstant3 = 21000.0f;
    const float kConstant4 = 24000.0f;
}

CubeMap::CubeMap(RawModel model, Shader shader, Texture3D day_texture, Texture3D night_texture) :
    model_(model), shader_(shader), day_texture_(day_texture), night_texture_(night_texture) {
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
