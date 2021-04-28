//
// Created by Ludwig on 5/28/2020.
//

#include "camera.h"
#include <glm/ext.hpp>

camera::camera(glm::vec3 cameraPos, float yaw, float pitch): cameraPos(cameraPos), cameraUp(0.0f, 1.0f, 0.0f), yaw(yaw), pitch(pitch) {
    updateRotation();
}

void camera::updateRotation() {
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

void camera::rotate(float x, float y) {
    this->yaw += x;
    this->pitch += y;
    updateRotation();
}

glm::mat4 camera::getViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void camera::moveForward(float speed) {
    cameraPos += speed * cameraFront;
}

void camera::moveBackward(float speed) {
    moveForward(-speed);
}

void camera::moveRight(float speed) {
    cameraPos += speed * glm::normalize(glm::cross(cameraFront, cameraUp));
}

void camera::moveLeft(float speed) {
    moveRight(-speed);
}
