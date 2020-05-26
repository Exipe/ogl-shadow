//
// Created by Ludwig on 5/25/2020.
//

#ifndef OGL_SHADOW_LIGHT_H
#define OGL_SHADOW_LIGHT_H

#include <glm/glm.hpp>

struct Material {
    glm::vec3 specular;
    float shininess;
};

struct Light {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif //OGL_SHADOW_LIGHT_H
