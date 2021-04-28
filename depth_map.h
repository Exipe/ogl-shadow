//
// Created by Ludwig on 5/29/2020.
//

#ifndef OGL_SHADOW_DEPTH_MAP_H
#define OGL_SHADOW_DEPTH_MAP_H

#include <glad/glad.h>
#include "texture/texture.h"

class DepthMap {
public:
    GLuint fbo;
    Texture texture;

    void bind() const;
    void unbind() const;
    void bindTexture(int unit = 0) const;
};

DepthMap initDepthMap(int shadowWidth, int shadowHeight);

#endif //OGL_SHADOW_DEPTH_MAP_H
