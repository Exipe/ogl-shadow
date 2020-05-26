//
// Created by Ludwig on 5/26/2020.
//

#ifndef OGL_SHADOW_MODEL_H
#define OGL_SHADOW_MODEL_H

#include <vector>
#include <glm/glm.hpp>
#include "texture.h"
#include "shader.h"

struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texPosition;
};

class Mesh {
private:
    GLuint vao;
    GLsizei vertices;

    Texture texture;
    Material material;

public:
    explicit Mesh(std::vector<VertexData> &vertices, Texture texture, Material material);
    void render(Program program) const;
};

class Model {
private:
    std::vector<Mesh> meshes;

public:
    Model();
    explicit Model(std::vector<Mesh> &meshes);
    void render(Program program);
};

bool loadModel(Model &model, const char *directory, const char *fileName);

#endif //OGL_SHADOW_MODEL_H
