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
                fov(45.0f),
                aspectRatio(1.33f) {}
    
    void setProjection(float posX, float posY, float posZ, float lookX, float lookY, float lookZ);

    Ray generateRay(float u, float v) const;
};