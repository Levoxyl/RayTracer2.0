// material.h
#pragma once
#include "vector3.h"

struct Material {
    Vector3 albedo;
    float specular = 0.5f;
    float shininess = 128.0f;
};