#ifndef TEXTURED_OBJECT_H
#define TEXTURED_OBJECT_H

#include "game_object.h"

struct TexturedObject : public GameObject {
    TexturedObject(RawModel model, Shader shader, Texture2D diffuse, Texture2D specular);

    void draw(glm::mat4& projection, glm::mat4& view) override;
    
private:
    Texture2D diffuse_;
    Texture2D specular_;
};

#endif
