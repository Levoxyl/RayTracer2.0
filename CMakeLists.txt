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
)

# MSVC dependency path

target_compile_definitions(RaytracerCore PRIVATE RAYTRACER_EXPORTS)

# Only copy DLLs on Windows
if(WIN32)
    add_custom_command(TARGET RaytracerCore POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${MINGW_PATH}/libgcc_s_seh-1.dll"
            "${CMAKE_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${MINGW_PATH}/libstdc++-6.dll"
            "${CMAKE_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${MINGW_PATH}/libwinpthread-1.dll"
            "${CMAKE_BINARY_DIR}"
        COMMENT "Copying MinGW runtime DLLs"
    )
endif()

target_compile_definitions(RaytracerCore PRIVATE RAYTRACER_EXPORTS)