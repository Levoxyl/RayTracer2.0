#include "ray.h"

Vector3 Ray::pointAt(float t) const {
    return origin + (direction * t);
}