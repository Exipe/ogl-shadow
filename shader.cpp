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

Program::Program(GLuint program): program(program) {
    modelLoc = glGetUniformLocation(program, "model");
    viewLoc = glGetUniformLocation(program, "view");
    projectionLoc = glGetUniformLocation(program, "projection");
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
