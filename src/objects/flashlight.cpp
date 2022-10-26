#include "flashlight.h"

Flashlight::Flashlight() :
    spot_light_(), toggled_(true) {}

void Flashlight::update(glm::vec3 pos, glm::vec3 dir) {
    spot_light_.position = pos;
    spot_light_.direction = dir;
}

void Flashlight::draw(Shader shader, glm::vec3& view_pos) {
    spot_light_.draw(shader, view_pos);
}

void Flashlight::toggle() {
    if(toggled_) {
	spot_light_.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	spot_light_.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    } else {
	spot_light_.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	spot_light_.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    
    toggled_ = !toggled_;
}
