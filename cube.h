//
// Created by Ludwig on 5/25/2020.
//

#ifndef OGL_SHADOW_CUBE_H
#define OGL_SHADOW_CUBE_H

#include <glad/glad.h>
#include "texture.h"
#include "light.h"
#include "shader.h"

class Cube {
private:
    GLuint vao;
    Material material;
    Texture texture;

public:
    Cube(GLuint vao, Material material, Texture texture);
    void render(Program program) const;
};

Cube initCube();

#endif //OGL_SHADOW_CUBE_H
