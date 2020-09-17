// VroemVroem - A 3D Highway simulator school project
// Made by Bastiaan van der Plaat (https://bastiaan.ml/)

//  Load standard c++ library
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>

// Load glad to load modern OpenGL context
#include "glad.h"

// Load glfw3 for window creation
#include <GLFW/glfw3.h>

// Load stb image for simple jpeg image loading
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#include "stb_image.h"

// Utils

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

int seed;
double random() {
    double x = sin(seed++) * 10000;
    return x - floor(x);
}

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

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
        int minX = MAX(mx, px);
        int minY = MAX(my, py);

        int maxX = MIN(mx + mode->width, px + sx);
        int maxY = MIN(my + mode->height, py + sy);

        // Calculate area of the intersection
        int area = MAX(maxX - minX, 0) * MAX(maxY - minY, 0);

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

// Math

// Vector 3
struct Vector3 {
    float x;
    float y;
    float z;

    Vector3 &operator +=(const Vector3 &b);
};

Vector3 &Vector3::operator +=(const Vector3 &b) {
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

// Matrix4
struct Matrix4 {
    float data[4][4];

    void identity();
    void perspective(float fov, float aspect, float near, float far);
    void translate(Vector3 *vector);
    void rotateX(float x);
    void rotateY(float y);
    void rotateZ(float z);
    void scale(Vector3 *vector);

    Matrix4 &operator *=(const Matrix4 &b);
};

void Matrix4::identity() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            data[y][x] = x == y;
        }
    }
}

void Matrix4::perspective(float fov, float aspect, float near, float far) {
    identity();
    float f = tan(M_PI * 0.5 - 0.5 * fov);
    float r = 1 / (near - far);
    data[0][0] = f / aspect;
    data[1][1] = f;
    data[2][2] = (near + far) * r;
    data[2][3] = -1;
    data[3][2] = near * far * r * 2;
    data[3][3] = 0;
}

void Matrix4::translate(Vector3 *vector) {
    identity();
    data[3][0] = vector->x;
    data[3][1] = vector->y;
    data[3][2] = vector->z;
}

void Matrix4::rotateX(float x) {
    identity();
    float c = cos(x);
    float s = sin(x);
    data[1][1] = c;
    data[1][2] = s;
    data[2][1] = -s;
    data[2][2] = c;
}

void Matrix4::rotateY(float y) {
    identity();
    float c = cos(y);
    float s = sin(y);
    data[0][0] = c;
    data[0][2] = -s;
    data[2][0] = s;
    data[2][2] = c;
}

void Matrix4::rotateZ(float z) {
    identity();
    float c = cos(z);
    float s = sin(z);
    data[0][0] = c;
    data[0][1] = s;
    data[1][0] = -s;
    data[1][1] = c;
}

void Matrix4::scale(Vector3 *vector) {
    identity();
    data[0][0] = vector->x;
    data[1][1] = vector->y;
    data[2][2] = vector->z;
}

Matrix4 &Matrix4::operator *=(const Matrix4 &b) {
    Matrix4 matrix;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            matrix.data[y][x] = 0;
            for (int i = 0; i < 4; i++) {
                matrix.data[y][x] += data[i][x] * b.data[y][i];
            }
        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            data[y][x] = matrix.data[y][x];
        }
    }

    return *this;
}

// Shader
class Shader {
    public:
        GLuint program;
        GLint position_attribute;
        GLint texture_position_attribute;
        GLint matrix_uniform;

        Shader(const char *vertex_path, const char *fragment_path);
};

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
        glGetShaderInfoLog(vertex_shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "[ERROR] Error compiling vertex shader: " << infoLog << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Compile fragment shader
    char *fragment_shader_text = file_read(fragment_path);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    delete fragment_shader_text;

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, sizeof(infoLog), NULL, infoLog);
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
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
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
class Texture {
    public:
        GLuint texture;

        Texture(const char *texture_path);
};

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
class Object3D {
    public:
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
        Matrix4 matrix;

        Object3D();

        virtual void updateMatrix();
};

Object3D::Object3D() {
    position = { 0, 0, 0 };
    rotation = { 0, 0, 0 };
    scale = { 1, 1, 1 };
    updateMatrix();
}

void Object3D::updateMatrix() {
    matrix.translate(&position);

    Matrix4 other_matrix;

    other_matrix.rotateX(rotation.x);
    matrix *= other_matrix;

    other_matrix.rotateY(rotation.y);
    matrix *= other_matrix;

    other_matrix.rotateZ(rotation.z);
    matrix *= other_matrix;

    other_matrix.scale(&scale);
    matrix *= other_matrix;
}

// PerspectiveCamera
class PerspectiveCamera : public Object3D {
    public:
        float fov;
        float aspect;
        float near;
        float far;

        PerspectiveCamera(float fov, float aspect, float near, float far);

        void updateMatrix();
};

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : fov(fov), aspect(aspect), near(near), far(far) {}

void PerspectiveCamera::updateMatrix() {
    matrix.perspective(fov, aspect, near, far);
    Matrix4 other_matrix;
    other_matrix.translate(&position);
    matrix *= other_matrix;
}

// Cube
class Cube : public Object3D {
    public:
        static GLuint vertex_array;
        static GLuint vertex_buffer;

        Texture *texture;

        static void init();

        Cube();

        void render(PerspectiveCamera *camera, Shader *shader);
};

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
    Matrix4 other_matrix = camera->matrix;
    other_matrix *= matrix;
    glUniformMatrix4fv(shader->matrix_uniform, 1, GL_FALSE, (const GLfloat *)other_matrix.data);

    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindVertexArray(vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// Game
class Game {
    public:
        GLFWwindow *window;
        PerspectiveCamera *camera;
        Vector3 velocity;
        Shader *shader;
        std::vector<Cube *> cubes;

        bool move_forward = false;
        bool move_left = false;
        bool move_right = false;
        bool move_backward = false;

        bool wireframe_mode = false;

        Game();

        void update(float delta);
        void render();
        void start();
};

// Work around ugly
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    Game *game = (Game *)glfwGetWindowUserPointer(window);

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

Game::Game() {
    // Init random number generator
    seed = time(NULL);

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
    window = glfwCreateWindow(1280, 720, "VroemVroem", nullptr, nullptr);
    if (!window) {
        std::cerr << "[ERROR] Can't create glfw window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowCenter(window);
    glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

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
    camera->position.z = -100;
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
