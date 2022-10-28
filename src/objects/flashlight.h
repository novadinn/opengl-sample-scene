#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H

#include "spot_light.h"

struct Flashlight {
    Flashlight();

    void update(glm::vec3 pos, glm::vec3 dir);

    void toggle();

    SpotLight spot_light;
private:
    bool toggled_;
};

#endif
