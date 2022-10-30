#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shader.h"
#include "../graphics/textures.h"

struct GameObject {
    GameObject(Shader shader);
    virtual ~GameObject() {};
    
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotation; // FIXME: use a quaternion instead

protected:
    Shader shader_;
};

#endif
