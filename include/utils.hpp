// VroemVroem - Utils Header

#pragma once

#include <GLFW/glfw3.h>

extern uint64_t seed;

double random();

char *file_read(const char *path);

bool glfwSetWindowCenter(GLFWwindow *window);
