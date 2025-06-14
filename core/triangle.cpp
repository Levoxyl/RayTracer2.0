#include "triangle.h"

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Material mat)
    : v0(a), v1(b), v2(c), material(mat) {}