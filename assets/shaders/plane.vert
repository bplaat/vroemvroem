#version 330 core

in vec4 position;
in vec2 texture_position;
uniform mat4 matrix;

out vec2 a_texture_position;

void main() {
    gl_Position = position * matrix;

    a_texture_position = texture_position;
}
