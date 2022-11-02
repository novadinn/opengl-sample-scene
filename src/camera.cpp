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
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch) :
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    movement_speed(kDefaultSpeed), mouse_sensitivity(kDefaultSensitivity), zoom(kDefaultZoom) {
    position = glm::vec3(pos_x, pos_y, pos_z);
    world_up = glm::vec3(up_x, up_y, up_z);
    yaw_ = yaw;
    pitch_ = pitch;
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

void Camera::moveGlobalUp(float delta_time) {
    float velocity = movement_speed * delta_time;
    position += world_up * velocity;
}

void Camera::moveGlobalDown(float delta_time) {
    float velocity = movement_speed * delta_time;
    position -= world_up * velocity;
}

void Camera::moveGlobalForward(float delta_time) {
    float velocity = movement_speed * delta_time;
    glm::vec3 new_front = glm::cross(world_up, right);
    position += new_front * velocity;
}

void Camera::moveGlobalBackward(float delta_time) {
    float velocity = movement_speed * delta_time;
    glm::vec3 new_front = glm::cross(world_up, right);
    position -= new_front * velocity;
}

void Camera::processMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw_   += x_offset;
    pitch_ += y_offset;

    if (constrain_pitch) {
	if (pitch_ > 89.0f) pitch_ = 89.0f;
	if (pitch_ < -89.0f) pitch_ = -89.0f;
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
    new_front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    new_front.y = sin(glm::radians(pitch_));
    new_front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front = glm::normalize(new_front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::setPitch(float pitch) {
    pitch_ = pitch;
    updateCameraVectors();
}

float Camera::getPitch() const {
    return pitch_;
}

void Camera::setYaw(float yaw) {
    yaw_ = yaw;
    updateCameraVectors();
}

float Camera::getYaw() const {
    return yaw_;
}
