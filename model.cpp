//
// Created by Ludwig on 5/26/2020.
//

#include <iostream>
#include <glad/glad.h>
#include "model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Mesh::Mesh(std::vector<VertexData> &vertices, Texture texture, Material material): texture(texture), material(material) {
    GLuint vbo, varray;

    glGenVertexArrays(1, &varray);
    glBindVertexArray(varray);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);

    //vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) offsetof(VertexData, position));
    glEnableVertexAttribArray(0);

    //vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) offsetof(VertexData, normal));
    glEnableVertexAttribArray(1);

    //vertex texture position
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*) offsetof(VertexData, texPosition));
    glEnableVertexAttribArray(2);

    this->vao = varray;
    this->vertices = vertices.size();
    this->material = material;
}

void Mesh::render(StandardProgram *program) const {
    texture.bind();
    program->setMaterial(material);

    render();
}

void Mesh::render() const {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices);
}

Mesh loadMesh(const tinyobj::attrib_t &attrib, const tinyobj::material_t &material, const tinyobj::mesh_t &mesh) {
    std::vector<VertexData> vertices(mesh.indices.size());

    auto texture = loadTexture(("texture/" + material.diffuse_texname).c_str());
    auto shininess = material.shininess;
    glm::vec3 specular(material.specular[0], material.specular[1], material.specular[2]);

    //For every vertex in the mesh
    for(size_t i = 0; i < mesh.indices.size(); i++) {
        VertexData vertexData {};
        auto idx = mesh.indices[i];

        vertexData.position.x = attrib.vertices[idx.vertex_index * 3 + 0];
        vertexData.position.y = attrib.vertices[idx.vertex_index * 3 + 1];
        vertexData.position.z = attrib.vertices[idx.vertex_index * 3 + 2];

        vertexData.normal.x = attrib.normals[idx.normal_index * 3 + 0];
        vertexData.normal.y = attrib.normals[idx.normal_index * 3 + 1];
        vertexData.normal.z = attrib.normals[idx.normal_index * 3 + 2];

        vertexData.texPosition.x = attrib.texcoords[idx.texcoord_index * 2 + 0];
        vertexData.texPosition.y = attrib.texcoords[idx.texcoord_index * 2 + 1];

        vertices[i] = vertexData;
    }

    return Mesh(vertices, texture, { specular, shininess });
}

Model::Model(std::vector<Mesh> &meshes): meshes(meshes) {}
Model::Model() = default;

void Model::render(StandardProgram *program) {
    for(Mesh m : meshes) {
        m.render(program);
    }
}

void Model::render() {
    for(Mesh m : meshes) {
        m.render();
    }
}

bool loadModel(Model &model, const char *directory, const char *fileName) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    auto success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, fileName, directory, true);

    if(!warn.empty()) {
        std::cout << "Warning: " << warn << std::endl;
    }

    if(!err.empty()) {
        std::cout << "Error: " << err << std::endl;
    }

    if(!success) {
        std::cout << "Failed to load model (" << fileName << ")." << std::endl;
        return false;
    }

    std::vector<Mesh> meshes;

    for(auto & shape : shapes) {
        auto matId = shape.mesh.material_ids[0];
        meshes.push_back(loadMesh(attrib, materials[matId], shape.mesh));
    }

    model = Model(meshes);
    return true;
}
