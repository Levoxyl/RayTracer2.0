import os
import sys
import ctypes

def check_dependencies():
    base_dir = os.path.dirname(os.path.abspath(__file__))
    dll_path = os.path.join(base_dir, "..", "build", "libRaytracerCore.dll")
    
    if not os.path.exists(dll_path):
        print(f"Error: DLL not found at {dll_path}")
        return False
    
    # Try loading DLL
    try:
        dll = ctypes.CDLL(dll_path)
        print("DLL loaded successfully!")
        return True
    except Exception as e:
        print(f"DLL loading error: {e}")
    
    # Check dependencies
    deps = [
        "libgcc_s_seh-1.dll",
        "libstdc++-6.dll",
        "libwinpthread-1.dll"
    ]
    
    print("\nChecking dependencies:")
    for dep in deps:
        try:
            ctypes.CDLL(dep)
            print(f"  {dep}: Found")
        except:
            print(f"  {dep}: Missing")
    
    return False

if __name__ == "__main__":
    if not check_dependencies():
        print("\nTroubleshooting steps:")
        print("1. Copy these from MinGW bin to your build directory:")
        print("   - libgcc_s_seh-1.dll")
        print("   - libstdc++-6.dll")
        print("   - libwinpthread-1.dll")
        print("2. Ensure Python architecture matches DLL (64-bit recommended)")