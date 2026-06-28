#pragma once
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include "triangle.h"
#include "ray.h"
#include "vector3.h"
#include "material.h"
#include <camera.h>

#ifdef _WIN32
#ifdef RAYTRACER_EXPORTS
#define RAYTRACER_API __declspec(dllexport)
#else
#define RAYTRACER_API __declspec(dllimport)
#endif
#else
#define RAYTRACER_API
#endif

struct HitRecord {
    HitRecord() : t(-1.f), point(), normal(), material() {}

    float t;
    Vector3 point;
    Vector3 normal;
    Material material;
};

class RAYTRACER_API Raytracer {

public:
    void setCamera(const Vector3& pos, const Vector3& target);

private:
    Camera camera;

public:
    Raytracer();

    void loadModel(const std::string& path);
    void render(int width, int height,
                std::function<void(int, int, float, float, float)> pixelCallback,
                std::function<void(float)> progressCallback);

private:
    std::vector<Triangle> triangles;

    bool intersectTriangle(const Triangle& tri,
                           const Ray& ray,
                           HitRecord& hit);
    Vector3 trace(const Ray& ray,
                  int depth = 0);

    void addDefaultScene();
};