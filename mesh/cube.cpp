//
// Created by Ludwig on 5/25/2020.
//

#include "cube.h"

float cube_vertices[] = {
        -1, -1, 1,
        -1, 1, 1,
        1, 1, 1,
        1, -1, 1,
        -1, -1, -1,
        -1, 1, -1,
        1, 1, -1,
        1, -1, -1
};

GLuint cube_elements[] = {
        0, 1, 2, 0, 2, 3,
        2, 3, 7, 2, 6, 7,
        4, 5, 6, 4, 6, 7,
        0, 1, 4, 1, 4, 5,
        0, 3, 4, 3, 4, 7,
        1, 2, 5, 2, 5, 6
};

Cube::Cube(GLuint vao, GLuint textureId): vao(vao), textureId(textureId) {}

void Cube::render(SkyProgram program) const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);
}

Cube initCube() {
    GLuint vbo, ebo, vao;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    auto texture = initCubeMap("texture/sky");
    return {vao, texture};
}
