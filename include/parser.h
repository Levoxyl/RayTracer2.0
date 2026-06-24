#pragma once
#include "triangle.h"
#include "material.h"
#include <vector>
#include <string>

class Parser {
public:
	static std::vector<Triangle> loadModel(const std::string& path,
												const Material& mat);
};