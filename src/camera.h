#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "glm/glm.hpp"

#include <vector>

namespace {
    const float kDefaultYaw = -90.0f;
    const float kDefaultPitch = 0.0f;
}

class Camera { // TODO: add view frustrum vars
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
	   float yaw = kDefaultYaw, float pitch = kDefaultPitch);
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

    glm::mat4 getProjectionMatrix(float width, float height, float near, float far) const;
    glm::mat4 getViewMatrix() const;
    
    void moveForward(float delta_time);
    void moveBackward(float delta_time);
    void moveLeft(float delta_time);
    void moveRight(float delta_time);
    void moveUp(float delta_time);
    void moveDown(float delta_time);
    void processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
    void processMouseScroll(float y_offset);

    void updateCameraVectors();

    void setPitch(float pitch);
    float getPitch() const;
    void setYaw(float yaw);
    float getYaw() const;
    
    glm::vec3 position;
    glm::vec3 world_up;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
    
private:
    float yaw_;
    float pitch_;
};

#endif
