//
// Created by Ludwig on 5/25/2020.
//

#include "shader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/ext.hpp>

std::string loadFileContent(const char* fileName) {
    std::ifstream file(fileName);
    if (!file) {
        std::cout << "File " << fileName << " not found." << std::endl;
        return std::string();
    }

    std::stringstream sstr;
    sstr << file.rdbuf();
    file.close();

    return sstr.str();
}

GLuint compileShader(const char *fileName, GLenum type) {
    auto stringSource = loadFileContent(fileName);
    const char *src = stringSource.c_str();

    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Error compiling shader (" << fileName << "): " << infoLog << std::endl;
    }

    return shader;
}

Program linkProgram(GLuint program) {
    glLinkProgram(program);

    GLint success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "Error linking program: " << infoLog << std::endl;
    }

    return Program(program);
}

GLuint Program::loc(const char *name) const {
    return glGetUniformLocation(program, name);
}

Program::Program(GLuint program): program(program) {
    modelLoc = loc("model");
    viewLoc = loc("view");
    projectionLoc = loc("projection");

    cameraLoc = loc("camera");

    matAmbLoc = loc("material.ambient");
    matDiffLoc = loc("material.diffuse");
    matSpecLoc = loc("material.specular");
    matShineLoc = loc("material.shininess");

    lightPosLoc = loc("light.position");
    lightAmbLoc = loc("light.ambient");
    lightDiffLoc = loc("light.diffuse");
    lightSpecLoc = loc("light.specular");
}

void Program::use() const {
    glUseProgram(program);
}

void Program::setModel(glm::mat4 model) const {
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Program::setView(glm::mat4 view) const {
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void Program::setProjection(glm::mat4 projection) const {
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Program::setCamera(glm::vec3 camera) {
    glUniform3fv(cameraLoc, 1, glm::value_ptr(camera));
}

void Program::setMaterial(Material mat) {
    glUniform3fv(matAmbLoc, 1, glm::value_ptr(mat.ambient));
    glUniform3fv(matDiffLoc, 1, glm::value_ptr(mat.diffuse));
    glUniform3fv(matSpecLoc, 1, glm::value_ptr(mat.specular));
    glUniform1f(matShineLoc, mat.shininess);
}

void Program::setLight(Light light) {
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.position));
    glUniform3fv(lightAmbLoc, 1, glm::value_ptr(light.ambient));
    glUniform3fv(lightDiffLoc, 1, glm::value_ptr(light.diffuse));
    glUniform3fv(lightSpecLoc, 1, glm::value_ptr(light.specular));
}
