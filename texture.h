//
// Created by Ludwig on 5/25/2020.
//

#ifndef OGL_SHADOW_TEXTURE_H
#define OGL_SHADOW_TEXTURE_H

#include <glad/glad.h>

class Texture {
private:
    GLuint texture;
    unsigned int width;
    unsigned int height;

public:
    Texture(GLuint texture, int width, int height);
    void bind(int unit = 0) const;
};

Texture loadTexture(const char *fileName);

#endif //OGL_SHADOW_TEXTURE_H
