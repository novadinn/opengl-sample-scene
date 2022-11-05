#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/shader.h"
#include "../graphics/textures.h"
#include "directional_light.h"
#include "spot_light.h"
#include "point_light.h"

struct GameObject {
    GameObject(Shader shader);
    virtual ~GameObject() {};

    void draw(glm::mat4& projection, glm::mat4& view);
    
    void useDirectionalLight(glm::vec3& view_pos, DirectionalLight& light);
    void usePointLight(glm::vec3& view_pos, PointLight& light);
    void useSpotLight(glm::vec3& view_pos, SpotLight& light);
    
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
