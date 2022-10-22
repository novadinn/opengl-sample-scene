#include "animation.h"

Animation::Animation(std::vector<Texture2D> sprite_sheet, float animation_speed) :
    timer_(animation_speed), current_frame_(0), sprite_sheet_(sprite_sheet) {}

void Animation::update(float delta_time) {
    timer_.update(delta_time);
    if(timer_.expired()) {
	current_frame_ = ((current_frame_+1) >= sprite_sheet_.size()) ?
	    0 : current_frame_ + 1;
	timer_.reset();
    }
}

void Animation::setupMainDrawing() {
    glActiveTexture(GL_TEXTURE0);
    sprite_sheet_[current_frame_].bind();
}

void Animation::stopDrawing() {
    sprite_sheet_[current_frame_].unbind();
}
