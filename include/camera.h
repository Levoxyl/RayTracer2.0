#pragma once
#include "ray.h"
#include "vector3.h"

struct Camera {
    Vector3 position;
    Vector3 lookAt;
    Vector3 up;
    float fov;
    float aspectRatio;
    
    Camera() : position(-1.0f, -0.5f, 0.0f),
                lookAt(1.0f, -0.5f, 0.0f),
                up(0.0f, 1.0f, 0.0f),
                fov(45),
                aspectRatio(1.33f) {}
    
    Ray generateRay(float u, float v) const;
};