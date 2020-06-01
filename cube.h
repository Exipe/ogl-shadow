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
    GLuint textureId;

public:
    Cube(GLuint vao, GLuint textureId);
    void render(SkyProgram program) const;
};

Cube initCube();

#endif //OGL_SHADOW_CUBE_H
