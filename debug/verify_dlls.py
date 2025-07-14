import os
import ctypes
from pathlib import Path

def verify_dlls():
    build_dir = Path(__file__).parent.parent / "build"
    required = [
        "libRaytracerCore.dll",
        "libgcc_s_seh-1.dll",
        "libstdc++-6.dll",
        "libwinpthread-1.dll"
    ]
    
    print("Verifying DLLs in:", build_dir)
    all_ok = True
    
    for dll in required:
        path = build_dir / dll
        if not path.exists():
            print(f"❌ Missing: {dll}")
            all_ok = False
            continue
            
        try:
            ctypes.CDLL(str(path))
            print(f"✅ {dll} - Loaded successfully")
        except Exception as e:
            print(f"❌ {dll} - Failed to load: {str(e)}")
            all_ok = False
            
    return all_ok

if __name__ == "__main__":
    if verify_dlls():
        print("\nAll DLLs verified successfully!")
    else:
        print("\nDLL verification failed - see errors above")
        print("Try copying MinGW DLLs from C:\\mingw64\\bin")