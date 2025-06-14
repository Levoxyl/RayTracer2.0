#pragma once
#include "ray.h"

struct Camera {
    Vector3 position;
    Vector3 target;
    Vector3 up;
    float fov;
    float aspectRatio;
    
    Ray generateRay(float px, float py) const;
};