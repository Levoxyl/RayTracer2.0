#pragma once
#include "ray.h"
#include "vector3.h"

struct Camera {
    Vector3 position;
    Vector3 lookAt;
    Vector3 up;
    float fov;
    float aspectRatio;
    
    Camera() : position(0.0f, 1.5f, 4.0f),
                lookAt(0.0f, 0.0f, 0.0f),
                up(0.0f, 1.0f, 0.0f),
                fov(45.0f) {}
    
    void setProjection(const Vector3& pos, const Vector3& target);

    Ray generateRay(float u, float v) const;
};