#pragma once
#include "vector3.h"
#include "material.h"

struct Triangle {
    Vector3 v0, v1, v2;
    Material material;
    
    Triangle(Vector3 a, Vector3 b, Vector3 c, Material mat);
};