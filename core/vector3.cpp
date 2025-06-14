#include "vector3.h"
#include <cmath>

Vector3 Vector3::operator+(const Vector3& v) const { 
    return Vector3(x+v.x, y+v.y, z+v.z); 
}

Vector3 Vector3::operator-(const Vector3& v) const { 
    return Vector3(x-v.x, y-v.y, z-v.z); 
}

Vector3 Vector3::operator*(float s) const { 
    return Vector3(x*s, y*s, z*s); 
}

Vector3 Vector3::operator/(float s) const { 
    return Vector3(x/s, y/s, z/s); 
}

Vector3 Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

float dot(const Vector3& a, const Vector3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

float length(const Vector3& v) {
    return std::sqrt(dot(v, v));
}

Vector3 normalize(const Vector3& v) {
    float len = length(v);
    return (len > 0) ? v / len : v;
}

Vector3 cross(const Vector3& a, const Vector3& b) {
    return Vector3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

Vector3 reflect(const Vector3& incident, const Vector3& normal) {
    return incident - normal * (2.0f * dot(incident, normal));
}