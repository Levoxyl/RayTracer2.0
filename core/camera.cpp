#include "camera.h"
#include <cmath>

void Camera::setProjection(const Vector3& pos, const Vector3& target) {

    lookAt.x = target.x;
    lookAt.y = target.y;
    lookAt.z = target.z;

    const float degToRad = 3.14159265f / 180.0f;
    float yaw = pos.x * degToRad;
    float pitch = pos.y * degToRad;

    if (pitch > 1.55f) pitch = 1.55f;
    if (pitch < -1.55f) pitch = -1.55f;

    float radius = pos.z;
    if (radius < 0.1f) radius = 0.1f;

    float offsetX = radius * cos(pitch) * sin(yaw);
    float offsetY = radius * sin(pitch);
    float offsetZ = radius * cos(pitch) * cos(yaw);

    position.x = lookAt.x + offsetX;
    position.y = lookAt.y + offsetY;
    position.z = lookAt.z + offsetZ;
        

    //Vector3 baseTarget(0.f, 0.f, 0.f);
    //Vector3 basePos(0.f, 1.5f, 4.f);

    //Vector3 fwd = Vector3::normalize(baseTarget - basePos);
    //Vector3 right = Vector3::normalize(Vector3::cross(fwd, up));
    //Vector3 trueUp = Vector3::cross(right, fwd);

    //position = basePos + (right * pos.x) + (trueUp * pos.y) + (fwd * (pos.z - 4.f));
    //
}

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