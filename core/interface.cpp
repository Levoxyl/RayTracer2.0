// core/interface.cpp
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "raytracer.h"
#include <vector>
#include <algorithm>

void save_image(const char* filename, const std::vector<float>& pixels, int w, int h) {
    std::vector<unsigned char> byte_pixels(w * h * 3);
    for(int i = 0; i < w * h * 3; i++) {
        float val = std::clamp(pixels[i], 0.0f, 1.0f);
        byte_pixels[i] = static_cast<unsigned char>(val * 255);
    }
    stbi_write_png(filename, w, h, 3, byte_pixels.data(), w * 3);
}

extern "C" __declspec(dllexport) void render_image(const char* filepath, int width, int height) {
    std::vector<float> pixels(width * height * 3, 0.0f);
    
    Raytracer r;
    r.loadModel(filepath);
    
    r.render(width, height,
        [&](int x, int y, float r, float g, float b) {
            int index = (y * width + x) * 3;
            pixels[index] = r;
            pixels[index+1] = g;
            pixels[index+2] = b;
        },
        [](float) {}
    );
    
    save_image("output.png", pixels, width, height);
}