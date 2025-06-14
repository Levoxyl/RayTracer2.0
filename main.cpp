// main.cpp
#include "raytracer.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <model_path> [width] [height]\n";
        return 1;
    }

    const std::string model_path = argv[1];
    const int width = (argc > 2) ? std::stoi(argv[2]) : 800;
    const int height = (argc > 3) ? std::stoi(argv[3]) : 600;

    Raytracer r;
    r.loadModel(model_path);
    
    std::vector<float> pixels(width * height * 3, 0.0f);
    
    r.render(width, height,
        [&](int x, int y, float r, float g, float b) {
            int index = (y * width + x) * 3;
            pixels[index] = r;
            pixels[index+1] = g;
            pixels[index+2] = b;
        },
        [](float) {}
    );
    
    // Save image will be handled by the GUI or separate function
    std::cout << "Rendering complete\n";
    return 0;
}