// VroemVroem - A 3D Highway simulator school project
// Made by Bastiaan van der Plaat (https://bastiaan.ml/)

#include "main.hpp"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "math.hpp"
#include "renderer.hpp"
#include "utils.hpp"

// Game

// Work around ugly I have to put these functions in the game class

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Game *game = (Game *)glfwGetWindowUserPointer(window);
    game->width = width;
    game->height = height;

    game->camera->aspect = (float)width / height;
    game->camera->updateMatrix();

    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    Game *game = (Game *)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
            game->move_forward = true;
        }
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            game->move_left = true;
        }
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            game->move_right = true;
        }
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
            game->move_backward = true;
        }
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_F11) {
            GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *primary_screen = glfwGetVideoMode(primary_monitor);

            if (!game->fullscreen_mode) {
                game->fullscreen_mode = true;
                glfwSetWindowMonitor(window, primary_monitor, 0, 0, primary_screen->width, primary_screen->height, GLFW_DONT_CARE);
            } else {
                game->fullscreen_mode = false;
                glfwSetWindowMonitor(window, nullptr, (primary_screen->width - GAME_WIDTH) / 2, (primary_screen->height - GAME_HEIGHT) / 2, GAME_WIDTH, GAME_HEIGHT, GLFW_DONT_CARE);
            }
        }

        if (key == GLFW_KEY_ESCAPE && game->cursor_locked) {
            game->cursor_locked = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        if (key == GLFW_KEY_Y) {
            game->wireframe_mode = !game->wireframe_mode;
            glPolygonMode(GL_FRONT_AND_BACK, game->wireframe_mode ? GL_LINE : GL_FILL);
        }

        if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
            game->move_forward = false;
        }
        if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            game->move_left = false;
        }
        if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            game->move_right = false;
        }
        if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
            game->move_backward = false;
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void)action;
    (void)mods;
    Game *game = (Game *)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !game->cursor_locked) {
        game->cursor_locked = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

// Temp / working on variables
float lastX = 1280 / 2, lastY = 720 / 2;
float yaw;
float pitch;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Game *game = (Game *)glfwGetWindowUserPointer(window);

    if (game->cursor_locked) {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.002;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f)  pitch = -89.0f;

        game->camera->rotation.x = cos(yaw) * cos(pitch);
        game->camera->rotation.y = sin(pitch);
        game->camera->rotation.z = sin(yaw);
    }
}

Game::Game() {
    // Init random number generator
    seed = time(nullptr);

    // Init glfw
    if (!glfwInit()) {
        std::cerr << "[ERROR] Can't init glfw" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Use OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create window
    width = GAME_WIDTH;
    height = GAME_HEIGHT;
    window = glfwCreateWindow(width, height, GAME_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "[ERROR] Can't create glfw window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *primary_screen = glfwGetVideoMode(primary_monitor);
    glfwSetWindowPos(window, (primary_screen->width - width) / 2, (primary_screen->height - height) / 2);

    glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Load OpenGL context
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Glad can't load OpenGL context" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);

    #ifdef DEBUG
    std::cout << "[INFO] Using OpenGL " << GLVersion.major << "."  << GLVersion.minor << " context" << std::endl;
    #endif

    // Create camera
    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    camera = new PerspectiveCamera(50, (float)window_width / window_height, 0.1, 1000);
    camera->position.z = -250;
    camera->updateMatrix();
    velocity = { 0, 0, 0 };

    // Load shader
    Cube::init();
    shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    // Load texture
    Texture crate_texture("assets/images/crate.jpg");

    // Create cubes
    for (int i = 0; i < 1000; i++) {
        Cube *cube = new Cube();
        cube->position.x = (random() - 0.5) * 250;
        cube->position.y = (random() - 0.5) * 250;
        cube->position.z = (random() - 0.5) * 250;
        cube->rotation.x = (random() - 0.5) * 2 * M_PI;
        cube->rotation.y = (random() - 0.5) * 2 * M_PI;
        cube->rotation.z = (random() - 0.5) * 2 * M_PI;
        cube->scale.x = random() * 9 + 1;
        cube->scale.y = random() * 9 + 1;
        cube->scale.z = random() * 9 + 1;
        cube->updateMatrix();
        cube->texture = &crate_texture;
        cubes.push_back(cube);
    }
}

void Game::update(float delta) {
    // Update player camera
    velocity.z -= velocity.z * 10 * delta;
    velocity.x -= velocity.x * 10 * delta;

    float speed = 7.5 * delta;

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

    camera->position += velocity;
    camera->updateMatrix();

    // Rotate cubes
    for (Cube *cube : cubes) {
        cube->rotation.x += 0.5 * delta;
        cube->rotation.y += 0.5 * delta;
        cube->updateMatrix();
    }
}

void Game::render() {
    glClearColor(0.53, 0.8, 0.92, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader->program);

    for (Cube *cube : cubes) {
        cube->render(camera, shader);
    }
}

void Game::start() {
    double lastTime = 0;
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        float delta = (float)(time - lastTime);
        lastTime = time;

        update(delta);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

int main() {
    Game game;
    game.start();
    return EXIT_SUCCESS;
}
