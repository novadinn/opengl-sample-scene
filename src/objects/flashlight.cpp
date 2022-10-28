#include "flashlight.h"

Flashlight::Flashlight() :
    spot_light(), toggled_(true) {}

void Flashlight::update(glm::vec3 pos, glm::vec3 dir) {
    spot_light.position = pos;
    spot_light.direction = dir;
}

void Flashlight::toggle() {
    if(toggled_) {
	spot_light.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	spot_light.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    } else {
	spot_light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	spot_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    
    toggled_ = !toggled_;
}
