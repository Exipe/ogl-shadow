//
// Created by Ludwig on 5/25/2020.
//
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint initCubeMap(std::string folder) {
    int width, height, nrChannels;
    GLubyte *imageData;

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    for(GLuint i = 0; i < 6; i++) {
        imageData = stbi_load((folder + "/" + std::to_string(1+i) + ".png").c_str(), &width, &height, &nrChannels, STBI_rgb);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        stbi_image_free(imageData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureId;
}

Texture::Texture(GLuint texture, int width, int height): texture(texture), width(width), height(height) {}

void Texture::bind(int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
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

    return Texture(texture, width, height);
}
