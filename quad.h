//
// Created by Ludwig on 5/29/2020.
//

#ifndef OGL_SHADOW_QUAD_H
#define OGL_SHADOW_QUAD_H

#include <glad/glad.h>

struct Quad {
    GLuint vao;
    void render() const;
};

Quad initQuad();

#endif //OGL_SHADOW_QUAD_H
