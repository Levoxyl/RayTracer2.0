#pragma once

struct Vector3 {
    float x, y, z;
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(float s) const;
    Vector3 operator/(float s) const;
    Vector3 operator-() const;
};

float dot(const Vector3& a, const Vector3& b);
float length(const Vector3& v);
Vector3 normalize(const Vector3& v);
Vector3 cross(const Vector3& a, const Vector3& b);
Vector3 reflect(const Vector3& incident, const Vector3& normal);