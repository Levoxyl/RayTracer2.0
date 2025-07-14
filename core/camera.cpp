#include "camera.h"
#include <cmath>

Ray Camera::generateRay(float u, float v) const {
    // Calculate camera basis vectors
    Vector3 w = Vector3::normalize(position - lookAt);
    Vector3 u_vec = Vector3::normalize(Vector3::cross(up, w));
    Vector3 v_vec = Vector3::cross(w, u_vec);
    
    // Calculate viewport dimensions
    float halfHeight = tan(fov * 0.5f * (3.14159f / 180.0f));
    float halfWidth = aspectRatio * halfHeight;
    
    // Calculate ray direction
    Vector3 direction = u_vec * ((u * 2.0f - 1.0f) * halfWidth) +
                        v_vec * ((v * 2.0f - 1.0f) * halfHeight) -
                        w;
    
    return Ray{position, Vector3::normalize(direction)};
}