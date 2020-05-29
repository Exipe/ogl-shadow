//
// Created by Ludwig on 5/29/2020.
//

#include <iostream>
#include "quad.h"

float quad_vertices[] = {
        -1, 1,
        1, 1,
        -1, -1,
        1, 1,
        1, -1,
        -1, -1
};

void Quad::render() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

Quad initQuad() {
    GLuint vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    return { vao };
}
