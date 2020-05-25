//
// Created by Ludwig on 5/25/2020.
//

#ifndef OGL_SHADOW_CUBE_H
#define OGL_SHADOW_CUBE_H

#include <glad/glad.h>

class Cube {
private:
    GLuint vao;

public:
    explicit Cube(GLuint vao);
    void render() const;
};

Cube initCube();

#endif //OGL_SHADOW_CUBE_H
