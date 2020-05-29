//
// Created by Ludwig on 5/29/2020.
//

#include "depth_map.h"

DepthMap initDepthMap(int shadowWidth, int shadowHeight) {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return { fbo, Texture(depthMap, shadowWidth, shadowHeight) };
}

void DepthMap::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void DepthMap::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthMap::bindTexture(int unit) const {
    texture.bind(unit);
}
