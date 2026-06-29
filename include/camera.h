#pragma once
#include "ray.h"
#include "vector3.h"

struct Camera {
    Vector3 position;
    Vector3 lookAt;
    Vector3 up;
    float fov;
    float aspectRatio = 1.0f;
    
    Camera() : position(0.0f, 5.17f, 19.31f), // Evaluated position for (Yaw: 0, Pitch: 15, Radius: 20)
                lookAt(0.0f, 0.0f, 0.0f),
                up(0.0f, 1.0f, 0.0f),
                fov(45.0f) {}
    
    void setProjection(const Vector3& pos, const Vector3& target);

    Ray generateRay(float u, float v) const;
};