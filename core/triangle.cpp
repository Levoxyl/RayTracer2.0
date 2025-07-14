// triangle.cpp
#include "triangle.h"

// IMPLEMENTATION ONLY (no declaration)
Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Material mat)
    : v0(a), v1(b), v2(c), material(mat) {
    calculateNormal();
}

void Triangle::calculateNormal() {
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    normal = Vector3::normalize(Vector3::cross(edge1, edge2));
}