// include/raytracer.h
#pragma once
#include <vector>
#include <string>
#include <functional>
#include "triangle.h"
#include "ray.h"

#ifdef _WIN32
    #ifdef RAYTRACER_EXPORTS
        #define RAYTRACER_API __declspec(dllexport)
    #else
        #define RAYTRACER_API __declspec(dllimport)
    #endif
#else
    #define RAYTRACER_API
#endif

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
    Vector3 cameraPosition = Vector3(0, 1.5, 4);
    
    // Core rendering functions
    Vector3 trace(const Ray& ray, int depth = 0);
    bool intersectTriangle(const Triangle& tri, const Ray& ray, struct HitRecord& hit);
    
    // Scene setup
    void addDefaultScene();
};