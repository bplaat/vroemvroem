// VroemVroem - A 3D Highway simulator school project
// Made by Bastiaan van der Plaat (https://bastiaan.ml/)

//  Load standard c++ library
#include <iostream>

// Load glad to load modern OpenGL context
#include "glad.h"

// Load glfw3 for window creation
#include <GLFW/glfw3.h>

// Load stb image for simple jpeg image loading
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#include "stb_image.h"

// Load linmat for simple matrix calculation
// I'm going to write my own C++ Matrix lib in the future
#include "linmat.h"

// Key down callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;

    // Close window on escape key pressed
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
}

// Main function
int main() {
    // Init glfw
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    // Use OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    auto window = glfwCreateWindow(1280, 720, "VroemVroem", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Register key down callback
    glfwSetKeyCallback(window, key_callback);

    // Create opengl context
    glfwMakeContextCurrent(window);

    // Load modern opengl
    if (!gladLoadGL()) {
        std::cerr << "[ERROR] Glad OpenGL loading error" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    std::cout << "[INFO] Using: OpenGL " << GLVersion.major << "."  << GLVersion.minor << std::endl;

    // Enable vsync
    glfwSwapInterval(1);

    // Compile vertex shader
    FILE *vertex_shader_file = fopen("assets/shaders/plane.vert", "rb");
    fseek(vertex_shader_file, 0, SEEK_END);
    size_t vertex_shader_file_size = ftell(vertex_shader_file);
    fseek(vertex_shader_file, 0, SEEK_SET);
    char *vertex_shader_text = new char[vertex_shader_file_size + 1];
    fread(vertex_shader_text, sizeof(char), vertex_shader_file_size, vertex_shader_file);
    vertex_shader_text[vertex_shader_file_size] = '\0';
    fclose(vertex_shader_file);

    auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);

    delete vertex_shader_text;

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "[ERROR] Error compiling vertex shader: " << infoLog << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Compile fragment shader
    FILE *fragment_shader_file = fopen("assets/shaders/plane.frag", "rb");
    fseek(fragment_shader_file, 0, SEEK_END);
    size_t fragment_shader_file_size = ftell(fragment_shader_file);
    fseek(fragment_shader_file, 0, SEEK_SET);
    char *fragment_shader_text = new char[fragment_shader_file_size + 1];
    fread(fragment_shader_text, sizeof(char), fragment_shader_file_size, fragment_shader_file);
    fragment_shader_text[fragment_shader_file_size] = '\0';
    fclose(fragment_shader_file);

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    delete fragment_shader_text;

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "[ERROR] Error compiling fragment shader: " << infoLog << std::endl;
        glfwTerminate();
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
        std::cerr << "[ERROR] Error linking program: " << infoLog << std::endl;
        glfwTerminate();
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
    float vertices[] = {
        // Vertex position
                   // Texture position
        -1,  1,    0, 1,
         1,  1,    1, 1,
        -1, -1,    0, 0,
         1, -1,    1, 0
    };
    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Get uniforms
    GLint matrix_location = glGetUniformLocation(program, "matrix");

    // Get attributes
    GLint position_location = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);

    GLint texture_position_location = glGetAttribLocation(program, "texture_position");
    glEnableVertexAttribArray(texture_position_location);
    glVertexAttribPointer(texture_position_location, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    // Load texture
    GLuint crate_texture;
    glGenTextures(1, &crate_texture);
    glBindTexture(GL_TEXTURE_2D, crate_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int crate_width, crate_height, crate_channels;
    unsigned char *crate_data = stbi_load("assets/images/crate.jpg", &crate_width, &crate_height, &crate_channels, 0);
    if (crate_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, crate_width, crate_height, 0, GL_RGB, GL_UNSIGNED_BYTE, crate_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(crate_data);
    } else {
        std::cerr << "[ERROR] Can't load crate image" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Set viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Clear and enable depth test
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.53, 0.8, 0.92, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use program
        glUseProgram(program);

        // Set model matrix
        mat4x4 matrix;
        mat4x4_identity(matrix);
        mat4x4_rotate_Z(matrix, matrix, (float)glfwGetTime());
        mat4x4_scale_aniso(matrix, matrix, 0.5, 0.5, 1);
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, (const GLfloat *)matrix);

        /// Draw rect
        glBindTexture(GL_TEXTURE_2D, crate_texture);
        glBindVertexArray(vertex_array);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        // Render frame
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
