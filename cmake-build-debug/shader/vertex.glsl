#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexPos;

void main() {
    gl_Position = projection * view * model * vec4(position.xyz, 1);
    fragNormal = mat3(transpose(inverse(model))) * normal;
    fragPos = vec3(model * vec4(position, 1));
    fragTexPos = texPosition;
}