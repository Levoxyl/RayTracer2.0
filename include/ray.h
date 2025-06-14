#pragma once
#include "vector3.h"

struct Ray {
    Vector3 origin;
    Vector3 direction;
    
    Vector3 pointAt(float t) const;
};