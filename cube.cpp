//
// Created by Ludwig on 5/25/2020.
//

#include "cube.h"

float cube_vertices[] = {
        -0.5, -0.5, 0.5, -1, -1, 1, 0, 0,
        -0.5, 0.5, 0.5, -1, 1, 1, 0, 1,
        0.5, 0.5, 0.5, 1, 1, 1, 1, 1,
        0.5, -0.5, 0.5, 1, -1, 1, 1, 0,
        -0.5, -0.5, -0.5, -1, -1, -1, 1, 0,
        -0.5, 0.5, -0.5, -1, 1, -1, 1, 1,
        0.5, 0.5, -0.5, 1, 1, -1, 0, 0,
        0.5, -0.5, -0.5, 1, -1, -1, 0, 1,
};

GLuint cube_elements[] = {
        0, 1, 2, 0, 2, 3,
        2, 3, 7, 2, 6, 7,
        4, 5, 6, 4, 6, 7,
        0, 1, 4, 1, 4, 5,
        0, 3, 4, 3, 4, 7,
        1, 2, 5, 2, 5, 6
};

Cube::Cube(GLuint vao, Material material, Texture texture): vao(vao), material(material), texture(texture) {}

void Cube::render(Program program) const {
    texture.bind();
    program.setMaterial(material);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    auto texture = loadTexture("texture/Crate.png");
    return Cube(vao, { glm::vec3(0, 0, 0), 1.0 }, texture);
}
