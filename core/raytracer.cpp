#include "raytracer.h"
#include "parser.h"
#include <algorithm>
#include <cmath>
#include <limits>   

Raytracer::Raytracer(){}

void Raytracer::setCamera(const Vector3& pos, const Vector3& target) {
    camera.setProjection(pos, target);
}

void Raytracer::loadModel(const std::string& path) {
     triangles.clear();

     Material modelMat;
     modelMat.albedo = Vector3(.0f, 1.f, .0f);
     modelMat.specular = .5f;
     modelMat.shininess = 32.f;
     
     triangles = Parser::loadModel(path, modelMat);

}

void Raytracer::render(int width, int height,
    std::function<void(int, int, float, float, float)>pixelCallback,
    std::function<void(float)> progressCallback) {
   
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float u = (x + 0.5f) / width;
            float v = 1.0f - ((y + 0.5f) / height);
             
            Ray ray = camera.generateRay(u, v);
            Vector3 color = trace(ray);

            pixelCallback(x, y, color.x, color.y, color.z);
        }
        if (progressCallback) {
            progressCallback(static_cast<float>(y) / height);
        }
    }
}

bool Raytracer::intersectTriangle(const Triangle& tri,
                                        const Ray& ray,
                                        HitRecord& hit) {
                                                            // Create traingle arrows
                                                            // Arrows != Vectors
                                                            // Arrows are a displacement
                                                            // Vectors are points 
    Vector3 edge1 = tri.v1 - tri.v0;                        // But an arrow                                                     // The - is not                            
    Vector3 edge2 = tri.v2 - tri.v0;                        // [point] <- [source]
                      
    Vector3 pvec = Vector3::cross(ray.direction, edge2);    // The Parallel Check
    float det = Vector3::dot(edge1, pvec);
                                                           
    if (fabs(det) < 0.000001f) {                            // True: Ray is parallel to the triangle plane                                                    
        return false;                                       // fabs = floating point absolute value
    }

    float invDet = 1.0f / det;
                               
    Vector3 tvec = ray.origin - tri.v0;                     // Barycentric Coordinates (u and v)
    float u = Vector3::dot(tvec, pvec) * invDet;            // tvec = translation vector
    if (u < 0.0f || u > 1.0f) {
        return false;                                       // Point falls outside the first edge boundary
    }

    Vector3 qvec = Vector3::cross(tvec, edge1);
    float v = Vector3::dot(ray.direction, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f) {
        return false;                                       // Point falls outside the diagonal boundary
    }
                                                            
    float t = Vector3::dot(edge2, qvec) * invDet;           // Distance (t)
    if (t < 0.0001f) {
        return false;                                       // Triangle is behind the camera
    }
                                                            // Update Hit
    hit.t = t;
    hit.point = ray.pointAt(t);
    hit.normal = tri.normal;
    hit.material = tri.material;

    return true;
}

void Raytracer::addDefaultScene() {
    triangles.clear();

    Material testMat;
    testMat.albedo = Vector3(0.8f, .2f, .2f);
    testMat.specular = .5f;
    testMat.shininess = 32.0f;

    Vector3 vertex0(-1.0f, -.5f, 0.0f);
    Vector3 vertex1(1.f, -.5f, .0f);
    Vector3 vertex2(.0f, 1.f, .0f);

    Triangle testTriangle(vertex0, vertex1, vertex2, testMat);

    triangles.push_back(testTriangle);

}

Vector3 Raytracer::trace(const Ray& ray, int depth) {
    HitRecord hit{};
    bool hitAnything = false;
    float closest = std::numeric_limits<float>::max();
    HitRecord closestHit{};
    
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
        Vector3 greySky(.75f, .75f, .78f);
        Vector3 greyGround(.85f, .85f, .85f);

        float groundY = -1.5f;
        if (ray.direction.y < .0f) {
            float tGround = (ray.origin.y - groundY) / -ray.direction.y;
            if (tGround > .001f) {
                Vector3 hitPoint = ray.pointAt(tGround);
                float pattern = (sin(hitPoint.x * 2.0f) * sin(hitPoint.z * 2.f) > .0f) ? 1.0f : 0.8f;
                return greyGround * pattern;
            }
        }

        return greySky;
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
    
    // SPECULAR HIGHLIGHTS
    Vector3 viewDir = Vector3::normalize(-ray.direction);
    Vector3 reflectDir = Vector3::reflect(-lightDir, normal);
    float spec = pow(
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