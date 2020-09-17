// VroemVroem - Math Header

#pragma once

// Vector3
struct Vector3 {
    float x;
    float y;
    float z;

    Vector3 &operator +=(const Vector3 &b);
};

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
