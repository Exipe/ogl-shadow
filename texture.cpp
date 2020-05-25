//
// Created by Ludwig on 5/25/2020.
//

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(GLuint texture, int width, int height): texture(texture), width(width), height(height) {}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

Texture loadTexture(const char *fileName) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte *imageData = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    return { texture, width, height };
}
