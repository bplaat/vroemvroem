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

// Read file to string
char *file_read(const char *path) {
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *text = new char[file_size + 1];
    fread(text, sizeof(char), file_size, file);
    text[file_size] = '\0';
    fclose(file);
    return text;
}


// BassieMath
struct Vector3 {
    float x;
    float y;
    float z;

    Vector3& operator +=(const Vector3& a) {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }
};

// Key down callback
bool move_forward = false;
bool move_left = false;
bool move_right = false;
bool move_backward = false;

bool wireframe_mode = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
            move_forward = true;
        }
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            move_left = true;
        }
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            move_right = true;
        }
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
            move_backward = true;
        }
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_Y) {
            wireframe_mode = !wireframe_mode;
        }

        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
            move_forward = false;
        }
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            move_left = false;
        }
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            move_right = false;
        }
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
            move_backward = false;
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

    glEnable(GL_DEPTH_TEST);

    // Compile vertex shader
    char *vertex_shader_text = file_read("assets/shaders/plane.vert");
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
    char *fragment_shader_text = file_read("assets/shaders/plane.frag");
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
        // Vertex position, Texture position
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // GLuint indices[] = {
    //     0, 1, 2,
    //     3, 7, 1,
    //     5, 4, 7,
    //     6, 2, 4,
    //     0, 1
    // };

    Vector3 cubePositions[] = {
        { 0.0f,  0.0f,  0.0f},
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // GLuint index_buffer;
    // glGenBuffers(1, &index_buffer);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Get uniforms
    GLint matrix_location = glGetUniformLocation(program, "matrix");

    // Get attributes
    GLint position_location = glGetAttribLocation(program, "position");
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(position_location);

    GLint texture_position_location = glGetAttribLocation(program, "texture_position");
    glVertexAttribPointer(texture_position_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(texture_position_location);

    // Load texture
    GLuint crate_texture;
    glGenTextures(1, &crate_texture);
    glBindTexture(GL_TEXTURE_2D, crate_texture);

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

    Vector3 camera = { 0, 0, -10 };
    Vector3 velocity = { 0, 0, 0 };

    float lastFrame = 0.0f;

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta
        float currentFrame = glfwGetTime();
        float delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update player camera
        velocity.z -= velocity.z * 10 * delta;
        velocity.x -= velocity.x * 10 * delta;

        auto speed = 3 * delta;

        if (move_forward) {
            velocity.z += speed;
        }
        if (move_left) {
            velocity.x -= speed / 2;
        }
        if (move_right) {
            velocity.x += speed / 2;
        }
        if (move_backward) {
            velocity.z -= speed;
        }

        camera += velocity;

        // Set viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Clear and enable depth test
        glPolygonMode(GL_FRONT_AND_BACK, wireframe_mode ? GL_LINE : GL_FILL);
        glClearColor(0.53, 0.8, 0.92, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use program
        glUseProgram(program);

        // Generate projection
        mat4x4 projection;
        mat4x4_identity(projection);
        mat4x4_perspective(projection, 75, (float)width / height, 0.1, 400);

        mat4x4 view;
        mat4x4_identity(view);
        mat4x4_translate(view, camera.x, camera.y, camera.z);

        glBindTexture(GL_TEXTURE_2D, crate_texture);
        glBindVertexArray(vertex_array);

        for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(Vector3); i++) {
            // Set matrix
            mat4x4 model;
            mat4x4_translate(model, cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
            mat4x4_rotate_X(model, model, (float)glfwGetTime() + 20 * i);
            mat4x4_rotate_Y(model, model, (float)glfwGetTime() + 20 * i);

            mat4x4 matrix;
            mat4x4_mul(matrix, view, model);
            mat4x4_mul(matrix, projection, matrix);
            glUniformMatrix4fv(matrix_location, 1, GL_FALSE, (const GLfloat *)matrix);

            // glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Render frame
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}
