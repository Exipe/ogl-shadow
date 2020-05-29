#version 330 core

in vec2 fragTexPos;

uniform sampler2D depthMap;

void main() {
    float value = texture(depthMap, fragTexPos).r;
    gl_FragColor = vec4(value, value, value, 1.0);
}