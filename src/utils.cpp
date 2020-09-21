// VroemVroem - Utils

#include "utils.hpp"
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

// Random
Random::Random(uint64_t seed) : seed(seed) {}

double Random::random() {
    double x = sin(seed++) * 10000;
    return x - floor(x);
}

// Read file to string
char *file_read(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == nullptr) {
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
