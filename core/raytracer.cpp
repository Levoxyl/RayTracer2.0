#include "raytracer.h"
#include <iostream>
#include <fstream>
#include <sstream>

Raytracer::Raytracer() {}

// ONLY ONE loadModel FUNCTION IMPLEMENTATION
void Raytracer::loadModel(const std::string& path) {
    std::cout << "Loading placeholder model: " << path << "\n";
    
    // Add a test triangle
    Material mat;
    mat.albedo = Vector3(1, 0, 0); // Red color
    
    triangles.push_back(Triangle(
        Vector3(-1, -1, 0),
        Vector3(1, -1, 0),
        Vector3(0, 1, 0),
        mat
    ));
    
    std::cout << "Added test triangle\n";
}

void Raytracer::render(int width, int height, 
                std::function<void(int, int, float, float, float)> pixelCallback,
                std::function<void(float)> progressCallback) 
{
    // Simple gradient for testing
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            float r = static_cast<float>(x) / width;
            float g = static_cast<float>(y) / height;
            float b = 0.5f;
            pixelCallback(x, y, r, g, b);
        }
        if(y % 10 == 0) {
            progressCallback(static_cast<float>(y) / height);
        }
    }
    progressCallback(1.0f);
}

// Other methods can be empty for now
void Raytracer::setCameraPosition(float x, float y, float z) {}
Vector3 Raytracer::trace(const Ray& ray, int depth) { return Vector3(); }
bool Raytracer::intersectTriangle(const Triangle& tri, const Ray& ray, struct HitRecord& hit) { return false; }
void Raytracer::addDefaultScene() {}