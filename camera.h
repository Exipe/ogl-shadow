//
// Created by Ludwig on 5/28/2020.
//

#ifndef OGL_SHADOW_CAMERA_H
#define OGL_SHADOW_CAMERA_H

#include <glm/glm.hpp>

class camera {
private:
    float yaw;
    float pitch;

    void updateRotation();

public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    explicit camera(glm::vec3 cameraPos, float yaw, float pitch);
    glm::mat4 getViewMatrix();

    void rotate(float x, float y);
    void moveForward(float speed);
    void moveBackward(float speed);
    void moveRight(float speed);
    void moveLeft(float speed);
};

#endif //OGL_SHADOW_CAMERA_H
