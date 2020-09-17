// VroemVroem - Renderer

#include "renderer.hpp"
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "math.hpp"
#include "utils.hpp"

// Shader
Shader::Shader(const char *vertex_path, const char *fragment_path) {
    // Read and compile vertex shader
    char *vertex_shader_text = file_read(vertex_path);
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);
    delete vertex_shader_text;

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "[ERROR] Error compiling vertex shader: " << infoLog << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Compile fragment shader
    char *fragment_shader_text = file_read(fragment_path);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);
    delete fragment_shader_text;

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "[ERROR] Error compiling fragment shader: " << infoLog << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Link program
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "[ERROR] Error linking program: " << infoLog << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Get attributes
    position_attribute = glGetAttribLocation(program, "position");
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(position_attribute);

    texture_position_attribute = glGetAttribLocation(program, "texture_position");
    glVertexAttribPointer(texture_position_attribute, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(texture_position_attribute);

    // Get uniforms
    matrix_uniform = glGetUniformLocation(program, "matrix");
}

// Texture
Texture::Texture(const char *texture_path) {
    int32_t texture_width, texture_height, texture_channels;
    uint8_t *texture_data = stbi_load("assets/images/crate.jpg", &texture_width, &texture_height, &texture_channels, STBI_rgb);
    if (!texture_data) {
        std::cerr << "[ERROR] Can't load texture: " << texture_path << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);
}

// Object3D
Object3D::Object3D() {
    position = { 0, 0, 0 };
    rotation = { 0, 0, 0 };
    scale = { 1, 1, 1 };
    updateMatrix();
}

void Object3D::updateMatrix() {
    matrix.translate(&position);

    Matrix4 temp_matrix;
    temp_matrix.rotateX(rotation.x);
    matrix *= temp_matrix;

    temp_matrix.rotateY(rotation.y);
    matrix *= temp_matrix;

    temp_matrix.rotateZ(rotation.z);
    matrix *= temp_matrix;

    temp_matrix.scale(&scale);
    matrix *= temp_matrix;
}

// PerspectiveCamera
PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : fov(fov), aspect(aspect), near(near), far(far) {}

void PerspectiveCamera::updateMatrix() {
    matrix.perspective(fov, aspect, near, far);

    Matrix4 view;
    view.translate(&position);

    Matrix4 temp_matrix;
    temp_matrix.rotateX(rotation.x);
    view *= temp_matrix;

    temp_matrix.rotateY(rotation.y);
    view *= temp_matrix;

    temp_matrix.rotateZ(rotation.z);
    view *= temp_matrix;

    temp_matrix.scale(&scale);
    view *= temp_matrix;

    matrix *= view;
}

// Cube
GLuint Cube::vertex_array = 0;
GLuint Cube::vertex_buffer = 0;

void Cube::init() {
    // Create vertext array
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    float vertices[] = {
        // Vertex position, Texture position
        -0.5, -0.5, -0.5,  0, 0,
        0.5, -0.5, -0.5,  1, 0,
        0.5,  0.5, -0.5,  1, 1,
        0.5,  0.5, -0.5,  1, 1,
        -0.5,  0.5, -0.5,  0, 1,
        -0.5, -0.5, -0.5,  0, 0,

        -0.5, -0.5,  0.5,  0, 0,
        0.5, -0.5,  0.5,  1, 0,
        0.5,  0.5,  0.5,  1, 1,
        0.5,  0.5,  0.5,  1, 1,
        -0.5,  0.5,  0.5,  0, 1,
        -0.5, -0.5,  0.5,  0, 0,

        -0.5,  0.5,  0.5,  1, 0,
        -0.5,  0.5, -0.5,  1, 1,
        -0.5, -0.5, -0.5,  0, 1,
        -0.5, -0.5, -0.5,  0, 1,
        -0.5, -0.5,  0.5,  0, 0,
        -0.5,  0.5,  0.5,  1, 0,

        0.5,  0.5,  0.5,  1, 0,
        0.5,  0.5, -0.5,  1, 1,
        0.5, -0.5, -0.5,  0, 1,
        0.5, -0.5, -0.5,  0, 1,
        0.5, -0.5,  0.5,  0, 0,
        0.5,  0.5,  0.5,  1, 0,

        -0.5, -0.5, -0.5,  0, 1,
        0.5, -0.5, -0.5,  1, 1,
        0.5, -0.5,  0.5,  1, 0,
        0.5, -0.5,  0.5,  1, 0,
        -0.5, -0.5,  0.5,  0, 0,
        -0.5, -0.5, -0.5,  0, 1,

        -0.5,  0.5, -0.5,  0, 1,
        0.5,  0.5, -0.5,  1, 1,
        0.5,  0.5,  0.5,  1, 0,
        0.5,  0.5,  0.5,  1, 0,
        -0.5,  0.5,  0.5,  0, 0,
        -0.5,  0.5, -0.5,  0, 1
    };

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

Cube::Cube() {
    texture = nullptr;
}

void Cube::render(PerspectiveCamera *camera, Shader *shader) {
    Matrix4 temp_matrix = camera->matrix;
    temp_matrix *= matrix;
    glUniformMatrix4fv(shader->matrix_uniform, 1, GL_FALSE, (const GLfloat *)temp_matrix.data);

    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
