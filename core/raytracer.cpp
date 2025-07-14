// raytracer.cpp
#include "raytracer.h"
#include <algorithm>  // For std::max and std::clamp
#include <cmath>      // For pow and sqrt
#include <limits>   

Vector3 Raytracer::trace(const Ray& ray, int depth) {
    HitRecord hit;
    bool hitAnything = false;
    float closest = std::numeric_limits<float>::max();
    HitRecord closestHit;
    
    for (const Triangle& tri : triangles) {
        if (intersectTriangle(tri, ray, hit)) {
            if (hit.t < closest) {
                closest = hit.t;
                closestHit = hit;
                hitAnything = true;
            }
        }
    }
    
    if (!hitAnything) {
        return Vector3(0.2f, 0.3f, 0.8f); // Blue background
    }

    Vector3 lightPos(2.0f, 5.0f, 3.0f);
    Vector3 toLight = lightPos - closestHit.point;
    Vector3 lightDir = Vector3::normalize(toLight);
    float lightDistance = toLight.length();

    // Normal handling
    Vector3 normal = closestHit.normal;
    if (Vector3::dot(normal, ray.direction) > 0) {
        normal = -normal;
    }
    
    // Diffuse shading
    float diff = std::max(0.0f, Vector3::dot(normal, lightDir));
    
    // Ambient lighting
    float ambient = 0.2f;
    
    // SPECULAR HIGHLIGHTS - ONLY ONE VERSION!
    Vector3 viewDir = Vector3::normalize(-ray.direction);
    Vector3 reflectDir = Vector3::reflect(-lightDir, normal);  // REMOVE THE OTHER DECLARATION
    float spec = pow(  // REMOVE THE OTHER DECLARATION
        std::max(0.0f, Vector3::dot(reflectDir, viewDir)), 
        closestHit.material.shininess
    );
    
    // Light attenuation
    float attenuation = 1.0f / (1.0f + 0.1f * lightDistance * lightDistance);
    diff *= attenuation;
    spec *= attenuation;
    
    // Combine lighting
    Vector3 color = closestHit.material.albedo * (ambient + diff) + 
                   Vector3(1,1,1) * spec * closestHit.material.specular;
    
    // Manual clamp (since std::clamp might not be available)
    color.x = (color.x < 0.0f) ? 0.0f : (color.x > 1.0f) ? 1.0f : color.x;
    color.y = (color.y < 0.0f) ? 0.0f : (color.y > 1.0f) ? 1.0f : color.y;
    color.z = (color.z < 0.0f) ? 0.0f : (color.z > 1.0f) ? 1.0f : color.z;
    
    return color;
}

// // raytracer.cpp
// Vector3 Raytracer::trace(const Ray& ray, int depth) {
//     HitRecord hit;
//     bool hitAnything = false;
//     float closest = std::numeric_limits<float>::max();
//     HitRecord closestHit;
    
//     for (const Triangle& tri : triangles) {
//         if (intersectTriangle(tri, ray, hit)) {
//             if (hit.t < closest) {
//                 closest = hit.t;
//                 closestHit = hit;
//                 hitAnything = true;
//             }
//         }
//     }
    
//     if (!hitAnything) {
//         return Vector3(0.2f, 0.3f, 0.8f);
//     }

//     // Point light setup
//     Vector3 lightPos(2.0f, 5.0f, 3.0f);
//     Vector3 toLight = lightPos - closestHit.point;
//     Vector3 lightDir = Vector3::normalize(toLight);
//     float lightDistance = toLight.length();

//     // Normal handling
//     Vector3 normal = closestHit.normal;
//     if (Vector3::dot(normal, ray.direction) > 0) {
//         normal = -normal;
//     }
    
//     // Diffuse shading
//     float diff = std::max(0.0f, Vector3::dot(normal, lightDir));
    
//     // Ambient lighting
//     float ambient = 0.2f;
    
//     // Specular highlight
//     Vector3 viewDir = Vector3::normalize(-ray.direction);
//     Vector3 reflectDir = Vector3::reflect(-lightDir, normal);
//     float spec = pow(
//         std::max(0.0f, Vector3::dot(reflectDir, viewDir)), 
//         closestHit.material.shininess
//     );
    
//     // Light attenuation
//     float attenuation = 1.0f / (1.0f + 0.1f * lightDistance * lightDistance);
//     diff *= attenuation;
//     spec *= attenuation;
    
//     // Combine lighting
//     Vector3 color = closestHit.material.albedo * (ambient + diff) + 
//                    Vector3(1,1,1) * spec * closestHit.material.specular;
    
//     // Clamp color values
//     color.x = std::clamp(color.x, 0.0f, 1.0f);
//     color.y = std::clamp(color.y, 0.0f, 1.0f);
//     color.z = std::clamp(color.z, 0.0f, 1.0f);
    
//     return color;
// }