cmake_minimum_required(VERSION 3.15)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Core rendering library (SHARED for DLL)
add_library(RaytracerCore SHARED
    core/ray.cpp
    core/camera.cpp
    core/interface.cpp
    core/raytracer.cpp
    core/triangle.cpp
    core/vector3.cpp
)

# Windows export handling
target_compile_definitions(RaytracerCore PRIVATE RAYTRACER_EXPORTS)

# Test executable - REMOVED (we'll focus on the DLL)
# add_executable(RayTracerTest main.cpp)
# target_link_libraries(RayTracerTest PRIVATE RaytracerCore)