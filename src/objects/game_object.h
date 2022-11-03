#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shader.h"
#include "../graphics/textures.h"

struct GameObject {
    GameObject(Shader shader);
    virtual ~GameObject() {};

    void draw(glm::mat4& projection, glm::mat4& view);
    
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotation; // FIXME: use a quaternion instead

protected:
    virtual void prepareDrawing();
    void setMVP(glm::mat4& projection, glm::mat4& view);
    void draw();
    virtual void endDrawing();
    
    Shader shader_;
};

#endif
