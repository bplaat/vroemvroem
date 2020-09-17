// VroemVroem - Renderer Header

#pragma once

#include "glad.h"
#include "math.hpp"

// Shader
class Shader {
    public:
        GLuint program;
        GLint position_attribute;
        GLint texture_position_attribute;
        GLint matrix_uniform;

        Shader(const char *vertex_path, const char *fragment_path);
};

// Texture
class Texture {
    public:
        GLuint texture;

        Texture(const char *texture_path);
};

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
