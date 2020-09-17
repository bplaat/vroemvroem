// VroemVroem - Math

#include "math.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

// Vector3
Vector3 &Vector3::operator +=(const Vector3 &b) {
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

// Matrix4
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
