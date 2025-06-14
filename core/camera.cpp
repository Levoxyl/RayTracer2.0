#include "camera.h"

Ray Camera::generateRay(float px, float py) const {
    return Ray{position, Vector3(px, py, 1)};
}