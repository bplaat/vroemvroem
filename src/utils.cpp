// VroemVroem - Utils

#include "utils.hpp"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

// Random
uint64_t seed;

double random() {
    double x = sin(seed++) * 10000;
    return x - floor(x);
}

// Read file to string
char *file_read(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        std::cerr << "[ERROR] Error reading file: " << path << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *contents = (char *)malloc(file_size + 1);
    fread(contents, sizeof(char), file_size, file);
    contents[file_size] = '\0';
    fclose(file);
    return contents;
}

bool glfwSetWindowCenter(GLFWwindow *window) {
    if (!window) {
        return false;
    }

    int sx = 0, sy = 0;
    int px = 0, py = 0;
    int mx = 0, my = 0;
    int monitor_count = 0;
    int best_area = 0;
    int final_x = 0, final_y = 0;

    glfwGetWindowSize(window, &sx, &sy);
    glfwGetWindowPos(window, &px, &py);

    // Iterate throug all monitors
    GLFWmonitor **m = glfwGetMonitors(&monitor_count);
    if (!m) {
        return false;
    }

    for (int j = 0; j < monitor_count; j++) {
        glfwGetMonitorPos(m[j], &mx, &my);
        const GLFWvidmode *mode = glfwGetVideoMode(m[j]);
        if (!mode) {
            continue;
        }

        // Get intersection of two rectangles - screen and window
        int minX = std::max(mx, px);
        int minY = std::max(my, py);

        int maxX = std::min(mx + mode->width, px + sx);
        int maxY = std::min(my + mode->height, py + sy);

        // Calculate area of the intersection
        int area = std::max(maxX - minX, 0) * std::max(maxY - minY, 0);

        // If its bigger than actual (window covers more space on this monitor)
        if (area > best_area) {
            // Calculate proper position in this monitor
            final_x = mx + (mode->width - sx) / 2;
            final_y = my + (mode->height - sy) / 2;

            best_area = area;
        }
    }

    // We found something
    if (best_area) {
        glfwSetWindowPos(window, final_x, final_y);
    }

    // Something is wrong - current window has NOT any intersection with any monitors. Move it to the default one.
    else {
        GLFWmonitor *primary = glfwGetPrimaryMonitor();
        if (primary) {
            const GLFWvidmode *desktop = glfwGetVideoMode(primary);
            if (desktop) {
                glfwSetWindowPos(window, (desktop->width - sx) / 2, (desktop->height - sy) / 2);
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}
