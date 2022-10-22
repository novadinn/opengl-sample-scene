#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/raw_model.h"
#include "../graphics/shader.h"
#include "../graphics/textures.h"

struct GameObject {
    GameObject(RawModel model, Shader shader, Texture2D texture);
    virtual ~GameObject() {};

    void draw(glm::mat4& projection, glm::mat4& view);
    
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotation; // FIXME: use a quaternion instead

protected:
    RawModel model_;
    Shader shader_;
    Texture2D texture_;
};

#endif
