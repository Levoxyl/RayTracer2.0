# Intro
This will be the more complex version of the raytracing project. It will support multiple types of 3D object formats.
There is a GUI where a file is chosen from the File Explorer instead of fixing the code directly. 
Instead of .ppm the output will be converted to .png by using stb_image. 
The software is made in VSCode with c++ 20 .

# Features
- Real-time ray tracing engine
- Multiple 3D object formats supported
- Python-based GUI
- Cross-platform support (Windows,Linux,MacOS)
- PNG output via STB library

# How to use
## System requirments 
- 4GB RAM (8GB recommended)
- 1GB free disk space
- Dedicated GPU for more complex scenes

## Development Tools Required
| Tool | Version | Installation |
|------|---------|--------------|
| **Git** | 2.35+ | [Download](https://git-scm.com/downloads) |
| **CMake** | 3.15+ | [Download](https://cmake.org/download/) |
| **MinGW-w64** | GCC 9.0+ | [Download](https://www.mingw-w64.org/downloads/) |
| **Python** | 3.8+ | [Download](https://www.python.org/downloads/) |
| **vcpkg** | Latest | [Instructions](#vcpkg-setup) |
| **Ninja** | 1.10+ | [Download](https://ninja-build.org/) |

## Setup guide
### 1. Clone Repository
```bash
git clone https://github.com/YourUsername/RayTracer2.0.git
cd RayTracer2.0
```

### 2. Install vcpkg
```powershell
# From project root
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
```

### 3. Install Dependencies
```powershell
# Install Assimp
.\vcpkg\vcpkg install assimp:x64-windows

# Set up vcpkg integration
.\vcpkg\vcpkg integrate install
```

### 4. Install Python Dependencies
```bash
pip install PyQt6
```

## Build Instructions

### Windows (MinGW)
```powershell
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build . --config Release
```

### Alternative Generator (Ninja)
```powershell
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM="path/to/ninja.exe"
cmake --build . --config Release
```

## Run the Application
```bash
# Run Python GUI
cd gui
python app.py
```

## Configuration Options
Set these in CMake:
```cmake
# Use different build system
cmake .. -G "Visual Studio 17 2022" -A x64

# Build debug version
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

## Troubleshooting
### Common Issues
1. **Missing DLL errors**:
   - Copy these from MinGW bin to build directory:
     - `libgcc_s_seh-1.dll`
     - `libstdc++-6.dll`
     - `libwinpthread-1.dll`

2. **vcpkg integration issues**:
   ```powershell
   .\vcpkg\vcpkg integrate remove
   .\vcpkg\vcpkg integrate install
   ```

3. **Python GUI not loading DLL**:
   - Verify DLL path in `gui/app.py`
   - Check architecture matches (64-bit Python for 64-bit build)
