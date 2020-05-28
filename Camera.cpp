//
// Created by Ludwig on 5/28/2020.
//

#include "Camera.h"
#include <glm/ext.hpp>

Camera::Camera(glm::vec3 cameraPos, float yaw, float pitch): cameraPos(cameraPos), cameraUp(0.0f, 1.0f, 0.0f), yaw(yaw), pitch(pitch) {
    updateRotation();
}

void Camera::updateRotation() {
    if(pitch > 90.0f) { //looking straight up
        pitch = 90.0f;
    }
    if(pitch < -90.0f) { //looking straight down
        pitch = -90.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
}

void Camera::rotate(float x, float y) {
    this->yaw += x;
    this->pitch += y;
    updateRotation();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::moveForward(float speed) {
    cameraPos += speed * cameraFront;
}

void Camera::moveBackward(float speed) {
    moveForward(-speed);
}

void Camera::moveRight(float speed) {
    cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::moveLeft(float speed) {
    moveRight(-speed);
}


