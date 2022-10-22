#include "camera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace {
    const float kDefaultSpeed =  2.5f;
    const float kDefaultSensitivity = 0.1f;
    const float kDefaultZoom = 45.0f;
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movement_speed(kDefaultSpeed), mouse_sensitivity(kDefaultSensitivity), zoom(kDefaultZoom) {
    position = position;
    world_up = up;
    yaw = yaw;
    pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movement_speed(kDefaultSpeed), mouse_sensitivity(kDefaultSensitivity), zoom(kDefaultZoom) {
    position = glm::vec3(pos_x, pos_y, pos_z);
    world_up = glm::vec3(up_x, up_y, up_z);
    yaw = yaw;
    pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getProjectionMatrix(float width, float height, float near, float far) const {
    return glm::perspective(glm::radians(zoom), width / height, near, far);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::moveForward(float delta_time) {
    float velocity = movement_speed * delta_time;
    position += front * velocity;
}

void Camera::moveBackward(float delta_time) {
    float velocity = movement_speed * delta_time;
    position -= front * velocity;
}

void Camera::moveLeft(float delta_time) {
    float velocity = movement_speed * delta_time;
    position -= right * velocity;
}

void Camera::moveRight(float delta_time) {
    float velocity = movement_speed * delta_time;
    position += right * velocity;
}

void Camera::moveUp(float delta_time) {
    float velocity = movement_speed * delta_time;
    position += world_up * velocity;
}

void Camera::moveDown(float delta_time) {
    float velocity = movement_speed * delta_time;
    position -= world_up * velocity;
}

void Camera::processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw   += x_offset;
    pitch += y_offset;

    if (constrain_pitch) {
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float y_offset) {
    zoom -= (float)y_offset;
    if (zoom < 1.0f) zoom = 1.0f;
    if (zoom > 45.0f) zoom = 45.0f; 
}

void Camera::updateCameraVectors() {
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
