#pragma once
#include <cmath>

struct Vector3 {
    float x, y, z;
    
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& v) const { 
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3 operator-(const Vector3& v) const { 
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3 operator*(float s) const { 
        return Vector3(x * s, y * s, z * s);
    }
    
    Vector3 operator/(float s) const { 
        return Vector3(x / s, y / s, z / s);
    }
    
    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }
    
    static float dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    static Vector3 normalize(const Vector3& v) {
        float len = std::sqrt(dot(v, v));
        return (len > 0) ? v / len : v;
    }
    
    static Vector3 cross(const Vector3& a, const Vector3& b) {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
    
    static Vector3 reflect(const Vector3& incident, const Vector3& normal) {
        return incident - normal * (2.0f * dot(incident, normal));
    }

    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};