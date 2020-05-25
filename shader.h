//
// Created by Ludwig on 5/25/2020.
//
// Utility function to deal with shader programs. Including useful debug output
//

#ifndef OGL_SHADOW_SHADER_H
#define OGL_SHADOW_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Program {
private:
    GLuint program;
    GLuint modelLoc;
    GLuint viewLoc;
    GLuint projectionLoc;

public:
    explicit Program(GLuint program);

    void use() const;
    void setModel(glm::mat4 model) const;
    void setView(glm::mat4 view) const;
    void setProjection(glm::mat4 projection) const;
};

GLuint compileShader(const char* fileName, GLenum type);
Program linkProgram(GLuint program);

#endif //OGL_SHADOW_SHADER_H
