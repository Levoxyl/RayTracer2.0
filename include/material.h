#pragma once
#include "vector3.h"

struct Material {
    Vector3 albedo = {0.8f, 0.8f, 0.8f};
    float roughness = 0.5f;
    float metallic = 0.0f;
};