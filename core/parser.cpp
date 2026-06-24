#include "parser.h"
#include <fstream>
#include <sstream>

std::vector<Triangle> Parser::loadModel(const std::string& path, const Material& mat) {
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;

    std::ifstream file(path);
    if (!file.is_open()) {
        return triangles;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back(Vector3(x, y, z));
        }
        else if (prefix == "f") {
            int idx0, idx1, idx2;
            ss >> idx0 >> idx1 >> idx2;

            // Convert OBJ 1-based indexing to C++ 0-based indexing
            Vector3 v0 = vertices[idx0 - 1];
            Vector3 v1 = vertices[idx1 - 1];
            Vector3 v2 = vertices[idx2 - 1];

            triangles.push_back(Triangle(v0, v1, v2, mat));
        }
    }

    return triangles;
}