#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "../graphics/spot_light.h"

struct Flashlight {
    Flashlight();

    void update(glm::vec3 pos, glm::vec3 dir);
    void draw(Shader shader, glm::vec3& view_pos);

    void toggle();
    
private:
    SpotLight spot_light_;
    bool toggled_;
};

#endif
