#ifndef WATER_H
#define WATER_H

#include "../graphics/resource_loader.h"
#include "game_object.h"

struct Water : public GameObject {
    Water(ResourceLoader& loader);

    void bindReflectionFrameBuffer();	
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();

    void draw(glm::mat4& projection, glm::mat4& view, glm::vec3 camera_position);
    void update(float delta_time);
    
    Texture2D getReflectionTexture() const;
    Texture2D getRefractionTexture() const;
    
    float wave_strength = 0.02f;
    float shine_damper = 20.0f;
    float reflectivity = 0.6f;
    
private:
    void prepareDrawing() override;
    void draw(glm::vec3 camera_position);
    void endDrawing() override;
    
    RawModel model_;
    
    FrameBuffer reflection_frame_buffer_;
    Texture2D reflection_texture_;
    uint reflection_depth_buffer_;

    FrameBuffer refraction_frame_buffer_;
    Texture2D refraction_texture_;
    Texture2D refraction_depth_texture_;

    Texture2D dudv_map_;
    Texture2D normal_map_;

    float move_factor_;
};

#endif
