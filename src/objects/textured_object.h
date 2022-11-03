#ifndef TEXTURED_OBJECT_H
#define TEXTURED_OBJECT_H

#include <vector>

#include "game_object.h"
#include "directional_light.h"
#include "spot_light.h"
#include "point_light.h"

struct TexturedObject : public GameObject {
    TexturedObject(RawModel model, Shader shader, std::vector<ObjectTexture> textures);

    void draw(glm::mat4& projection, glm::mat4& view,
	      glm::vec3& view_pos, DirectionalLight& dir_light,
	      SpotLight& spot_light, PointLight& point_light);
private:
    void draw(glm::vec3& view_pos, DirectionalLight& dir_light, SpotLight& spot_light, PointLight& point_light);
    void prepareDrawing() override;
    void endDrawing() override;
    
    std::vector<ObjectTexture> textures_;
    RawModel model_;
};

#endif
