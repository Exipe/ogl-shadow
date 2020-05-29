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

Program::Program(GLuint program): program(program) {
    link();
}

GLuint Program::loc(const char *name) const {
    auto location = glGetUniformLocation(program, name);
    //std::cout << name << " is at " << location << std::endl;
    return location;
}

void Program::link() const {
    glLinkProgram(program);

    GLint success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "Error linking program: " << infoLog << std::endl;
    }
}

void Program::use() const {
    glUseProgram(program);
}

StandardProgram::StandardProgram(GLuint program): Program(program) {
    modelLoc = loc("model");
    viewLoc = loc("view");
    projectionLoc = loc("projection");

    cameraLoc = loc("camera");

    matSpecLoc = loc("material.specular");
    matShineLoc = loc("material.shininess");

    lightPosLoc = loc("light.position");
    lightAmbLoc = loc("light.ambient");
    lightDiffLoc = loc("light.diffuse");
    lightSpecLoc = loc("light.specular");
}

void StandardProgram::setModel(glm::mat4 model) const {
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void StandardProgram::setView(glm::mat4 view) const {
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void StandardProgram::setProjection(glm::mat4 projection) const {
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void StandardProgram::setCamera(glm::vec3 camera) const {
    glUniform3fv(cameraLoc, 1, glm::value_ptr(camera));
}

void StandardProgram::setMaterial(Material mat) const {
    glUniform3fv(matSpecLoc, 1, glm::value_ptr(mat.specular));
    glUniform1f(matShineLoc, mat.shininess);
}

void StandardProgram::setLight(Light light) const {
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.position));
    glUniform3fv(lightAmbLoc, 1, glm::value_ptr(light.ambient));
    glUniform3fv(lightDiffLoc, 1, glm::value_ptr(light.diffuse));
    glUniform3fv(lightSpecLoc, 1, glm::value_ptr(light.specular));
}

DepthProgram::DepthProgram(GLuint program) : Program(program) {
    lightSpaceLoc = loc("lightSpace");
    modelLoc = loc("model");
}

void DepthProgram::setLightSpace(glm::mat4 lightSpace) const {
    glUniformMatrix4fv(lightSpaceLoc, 1, GL_FALSE, glm::value_ptr(lightSpace));
}

void DepthProgram::setModel(glm::mat4 model) const {
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

ShadowProgram::ShadowProgram(GLuint program) : StandardProgram(program) {
    lightSpaceLoc = loc("lightSpace");
    depthMapLoc = loc("depthMap");
}

void ShadowProgram::setLightSpace(glm::mat4 lightSpace) const {
    glUniformMatrix4fv(lightSpaceLoc, 1, GL_FALSE, glm::value_ptr(lightSpace));
}

void ShadowProgram::setDepthMap(int textureUnit) const {
    glUniform1i(depthMapLoc, textureUnit);
}