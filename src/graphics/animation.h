#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

#include "resource_loader.h"
#include "../timer.h"

struct Animation {
    Animation(std::vector<Texture2D> sprite_sheet, float animation_speed);

    void update(float delta_time);
    void setupMainDrawing();
    void stopDrawing();
private:
    std::vector<Texture2D> sprite_sheet_;
    uint current_frame_;
    Timer timer_;
};

#endif
