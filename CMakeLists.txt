# Metadata
cmake_minimum_required(VERSION 3.15)
project(RayTracer)

# C++ version
set(CMAKE_CXX_STANDARD 20 CACHE STRING "Force C++ Version" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MSVC to select the proper Debug vs Release CRT binaries automatically
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

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

# Match the correct debug configuration libraries explicitly
if(MSVC)
    target_link_libraries(RaytracerCore PRIVATE 
        $<$<CONFIG:Debug>:msvcrtd vcruntimed ucrtd>
        $<$<NOT:$<CONFIG:Debug>>:msvcrt vcruntime ucrt>
    )
endif()