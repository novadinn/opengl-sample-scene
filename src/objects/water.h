#ifndef WATER_H
#define WATER_H

#include "../graphics/resource_loader.h"
#include "game_object.h"

struct Water : public GameObject {
    Water(ResourceLoader& loader);

    void bindReflectionFrameBuffer();	
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();
    
    void draw(glm::mat4& projection, glm::mat4& view);

private:
    FrameBuffer reflection_frame_buffer_;
    Texture2D reflection_texture_;
    uint reflection_depth_buffer_;

    FrameBuffer refraction_frame_buffer_;
    Texture2D refraction_texture_;
    Texture2D refraction_depth_texture_;
};

#endif
