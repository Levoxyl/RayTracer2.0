#include "raytracer.h"
#include <algorithm>
#include <cmath>
#include <limits>   

Raytracer::Raytracer() {
    cameraPosition = Vector3(0.0f, 0.0f, 0.0f);
}

void Raytracer::loadModel(const std::string& path) {
    addDefaultScene();
}

void Raytracer::render(int width, int height,
    std::function<void(int, int, float, float, float)>pixelCallback,
    std::function<void(float)> progressCallback) {
   
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float u = (x + 0.5f) / width * 2.0f - 1.0f;
            float v = (y + 0.5f) / height * 2.0f - 1.0f;
             
            Ray ray(cameraPosition, Vector3(u, v, -1.0f));
            Vector3 color = trace(ray);

            pixelCallback(x, y, color.x, color.y, color.z);
        }
        if (progressCallback) {
            progressCallback(static_cast<float>(y) / height);
        }
    }
}

bool Raytracer::intersectTriangle(const Triangle& tri, const Ray& ray, HitRecord& hit) {
    return false;
}

void Raytracer::addDefaultScene() {

}

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