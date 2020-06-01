#version 330 core

layout(location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 projection;
uniform mat3 view;

void main() {
    texCoords = position;
    gl_Position = projection * mat4(view) * vec4(position, 1);
}