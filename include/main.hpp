// VroemVroem - Main Header

#pragma once

#include <iostream>
#include <vector>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "math.hpp"
#include "renderer.hpp"

#define GAME_TITLE "VroemVroem"
#define GAME_VERSION "0.1.0"

#define GAME_WIDTH 1280
#define GAME_HEIGHT 720

#define GAME_MIN_WIDTH 640
#define GAME_MIN_HEIGHT 480

class Game {
    public:
        GLFWwindow *window;

        PerspectiveCamera *camera;
        Vector3 velocity;

        Shader *shader;
        std::vector<Cube *> cubes;

        int width;
        int height;

        bool fullscreen_mode = false;
        bool wireframe_mode = false;
        bool cursor_locked = false;

        bool move_forward = false;
        bool move_left = false;
        bool move_right = false;
        bool move_backward = false;

        Game();

        void update(float delta);
        void render();
        void start();
};
