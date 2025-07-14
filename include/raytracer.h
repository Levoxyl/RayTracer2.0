#pragma once
#include <vector>
#include <string>
#include <functional>
#include <algorithm>  // Add for std::clamp
#include "triangle.h"
#include "ray.h"
#include "vector3.h"  // Add this
#include "material.h"  // Add this

#ifdef _WIN32
    #ifdef RAYTRACER_EXPORTS
        #define RAYTRACER_API __declspec(dllexport)
    #else
        #define RAYTRACER_API __declspec(dllimport)
    #endif
#else
    #define RAYTRACER_API
#endif

// Add this struct
struct HitRecord {
    float t;
    Vector3 point;
    Vector3 normal;
    Material material;
};

class RAYTRACER_API Raytracer {
public:
    Raytracer();
    
    void loadModel(const std::string& path);
    void render(int width, int height, 
                std::function<void(int, int, float, float, float)> pixelCallback,
                std::function<void(float)> progressCallback);
    
    void setCameraPosition(float x, float y, float z);
    
private:
    std::vector<Triangle> triangles;
    Vector3 cameraPosition;
    
    // Core rendering functions
     bool intersectTriangle(const Triangle& tri, const Ray& ray, HitRecord& hit);
    Vector3 trace(const Ray& ray, int depth = 0);
    
    // Scene setup
    void addDefaultScene();
};