//
// Created by Ludwig on 5/25/2020.
//
// Utility function to deal with shader programs. Including useful debug output
//

#ifndef OGL_SHADOW_SHADER_H
#define OGL_SHADOW_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "light.h"

class Program {
private:
    GLuint program;
    void link() const;

protected:
    GLuint loc(const char *name) const;

public:
    explicit Program(GLuint program);
    void use() const;
};

class StandardProgram: public Program {
private:
    GLuint modelLoc;
    GLuint viewLoc;
    GLuint projectionLoc;

    GLuint cameraLoc;
    GLuint matSpecLoc, matShineLoc;
    GLuint lightPosLoc, lightAmbLoc, lightDiffLoc, lightSpecLoc;

public:
    void setModel(glm::mat4 model) const;
    void setView(glm::mat4 view) const;
    void setProjection(glm::mat4 projection) const;

    void setCamera(glm::vec3 camera) const;
    void setMaterial(Material mat) const;
    void setLight(Light light) const;

    explicit StandardProgram(GLuint program);
};

class ShadowProgram: public StandardProgram {
private:
    GLuint lightSpaceLoc;
    GLuint depthMapLoc;

public:
    void setLightSpace(glm::mat4 lightSpace) const;
    void setDepthMap(int textureUnit) const;

    explicit ShadowProgram(GLuint program);
};

class DepthProgram: public Program {
private:
    GLuint lightSpaceLoc;
    GLuint modelLoc;

public:
    void setLightSpace(glm::mat4 lightSpace) const;
    void setModel(glm::mat4 model) const;

    explicit DepthProgram(GLuint program);
};

GLuint compileShader(const char* fileName, GLenum type);

#endif //OGL_SHADOW_SHADER_H
