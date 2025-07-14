#pragma once
#include "ray.h"
#include "vector3.h"

struct Camera {
    Vector3 position;
    Vector3 lookAt;
    Vector3 up;
    float fov;
    float aspectRatio;
    
    Camera() : position(0, 1.5, 4), lookAt(0, 0, 0), up(0, 1, 0), fov(45), aspectRatio(1.33f) {}
    
    Ray generateRay(float u, float v) const;
};