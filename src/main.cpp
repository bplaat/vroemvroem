// VroemVroem - A 3D Highway simulator school project
// Made by Bastiaan van der Plaat (https://bastiaan.ml/)

//  Load libs
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glad.h"

// Vertex shader
const char *vertex_shader_text = "#version 330 core\n \
in vec4 position;\n \
void main() {\n \
    gl_Position = position;\n \
}";

// Fragment shader
const char *fragment_shader_text = "#version 330 core\n \
out vec4 color;\n \
uniform float time;\n \
uniform vec2 resolution;\n \
void main() {\n \
    vec2 position = gl_FragCoord.xy / resolution;\n \
    color = vec4(resolution.x + 0.5, sin(position.x + time), cos(position.y + time), 1.0);\n \
}";

int main() {
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    // Use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    auto window = glfwCreateWindow(1280, 720, "VroemVroem", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Create opengl context
    glfwMakeContextCurrent(window);

    // Load modern opengl
    if (!gladLoadGL()) {
        std::cerr << "Glad error!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "[INFO] Using: OpenGL " << GLVersion.major << "."  << GLVersion.minor << std::endl;

    // Enable vsync
    glfwSwapInterval(1);

    // Compile vertex shader
    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, sizeof(infoLog), NULL, infoLog);
        std::cout << "Error compiling vertex shader: " << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    // Compile fragment shader
    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, sizeof(infoLog), NULL, infoLog);
        std::cout << "Error compiling fragment shader: " << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    // Link program
    auto program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        std::cout << "Error linking program: " << infoLog << std::endl;
        return EXIT_FAILURE;
    }

    // Delete shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Create vertext array
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // Create vertex buffer
    float vertices[] = { -1,1, -1,-1, 1,1,  1,-1, 1,1, -1,-1 };
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Get uniforms
    GLint time_uniform = glGetUniformLocation(program, "time");
    GLint resolution_uniform = glGetUniformLocation(program, "resolution");

    // Get attributes
    GLint position_location = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    // Game loop
    auto time = 0.0;
    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glUniform1f(time_uniform, time);
        time += 0.05;
        glUniform2f(resolution_uniform, width, height);

        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / 2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
