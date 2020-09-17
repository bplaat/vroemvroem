#version 330 core

in vec2 a_texture_position;
uniform sampler2D a_texture;

out vec4 color;

void main() {
    color = texture(a_texture, a_texture_position);
}
