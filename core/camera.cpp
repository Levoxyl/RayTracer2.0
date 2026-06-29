#include "camera.h"
#include <cmath>

void Camera::setProjection(const Vector3& pos, const Vector3& target) {

    lookAt.x = target.x;
    lookAt.y = target.y;
    lookAt.z = target.z;

    const float degToRad = 3.14159265f / 180.0f;

    float yaw = pos.x * degToRad;
    float pitch = pos.y * degToRad;

    if (pitch > 1.48f) pitch = 1.48f;
    if (pitch < -1.48f) pitch = -1.48f;

    float radius = pos.z;
    if (radius < 0.1f) radius = 0.1f;

    float offsetX = radius * cos(pitch) * sin(yaw);
    float offsetY = radius * sin(pitch);
    float offsetZ = radius * cos(pitch) * cos(yaw);

    position.x = lookAt.x + offsetX;
    position.y = lookAt.y + offsetY;
    position.z = lookAt.z + offsetZ;
}

Ray Camera::generateRay(float u, float v) const {

    Vector3 w = Vector3::normalize(position - lookAt);
    Vector3 localUp = up;

    if (std::abs(w.y) > 0.95f) {
        localUp = Vector3(.0f, .0f, 1.0f);
    }

    Vector3 u_vec = Vector3::normalize(Vector3::cross(localUp, w));
    Vector3 v_vec = Vector3::cross(w, u_vec);
    
    // viewport dimensions
    float halfHeight = tan(fov * 0.5f * (3.14159265f / 180.0f));
    float halfWidth = aspectRatio * halfHeight;
    
    // ray direction
    Vector3 direction = u_vec * ((u * 2.0f - 1.0f) * halfWidth) +
                        v_vec * ((v * 2.0f - 1.0f) * halfHeight) -
                        w;
    
    return Ray{position, Vector3::normalize(direction)};
}