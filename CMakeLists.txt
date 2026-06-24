# Metadata
cmake_minimum_required(VERSION 3.15)
project(RayTracer)

# C++ version
set(CMAKE_CXX_STANDARD 20 CACHE STRING "Force C++ Version" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(RaytracerCore SHARED
    core/ray.cpp
    core/camera.cpp
    core/interface.cpp
    core/raytracer.cpp
    core/triangle.cpp 
    core/parser.cpp
    include/parser.h
)

target_compile_definitions(RaytracerCore PRIVATE RAYTRACER_EXPORTS)