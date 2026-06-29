    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #pragma warning(push)
    #pragma warning(disable : 26819)
    #include "stb_image_write.h"
    #pragma warning(pop)
    #include "raytracer.h"
    #include <vector>
    #include <algorithm>
    #include <iostream>

    static void save_image(const char* filename, const std::vector<float>& pixels, int w, int h) {
        std::vector<unsigned char> byte_pixels(static_cast<size_t>(w) * h * 3);
        for (int i = 0; i < w * h * 3; i++) {
            float val = std::clamp(pixels[i], 0.0f, 1.0f);
            byte_pixels[i] = static_cast<unsigned char>(val * 255);
        }
        stbi_write_png(filename, w, h, 3, byte_pixels.data(), w * 3);
    }

    extern "C" {

        // Create the persistent engine instance on the heap
        __declspec(dllexport) Raytracer* create_raytracer() {
            return new Raytracer();
        }

        // Safely destroy the instance when Python exits
        __declspec(dllexport) void destroy_raytracer(Raytracer* r) {
            if (r != nullptr) {
                delete r;
            }
        }

        // Load the 3D model x1 into the persistent engine
        __declspec(dllexport) void load_model_to_engine(Raytracer* r, const char* filepath) {
            if (r == nullptr) {
                std::cout << "❌ C++ Error: Cannot load model. Raytracer pointer is null." << std::endl;
                return;
            }
            r->loadModel(filepath);
        }

        // Update camera vectors inside the engine without reloading geometry
        __declspec(dllexport) void set_engine_camera(Raytracer* r, const Vector3* pos, const Vector3* target) {
            if (r == nullptr || pos == nullptr || target == nullptr) {
                std::cout << "❌ C++ Error: Cannot set camera. Missing pointers." << std::endl;
                return;
            }
            r->setCamera(*pos, *target);
        }

        // Isolated render pass using whatever state currently lives in the engine
        __declspec(dllexport) void render_image(Raytracer* r,
                                                const char* output_path,
                                                int width,
                                                int height) {
            if (r == nullptr) {
                std::cout << "❌ C++ Error: Cannot render. Raytracer pointer is null." << std::endl;
                return;
            }

            std::vector<float> pixels(static_cast<size_t>(width) * height * 3, 0.0f);

            r->render(width, height,
                      [&](int x, int y, float r_val, float g_val, float b_val)
                      {
                          size_t index = (static_cast<size_t>(y) * width + x) * 3;
                          pixels[index] = r_val;
                          pixels[index + 1] = g_val;
                          pixels[index + 2] = b_val;
                      },
                      [](float) {}
            );

            save_image(output_path, pixels, width, height);
        }
    }