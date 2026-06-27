from http.client import FOUND
import os
import ctypes
from pathlib import Path

def verify_dlls():
    build_dir = Path(__file__).parent.parent / "out" / "build" / "x64-Debug"
    required = "RaytracerCore.dll"

    print("Verifying DLLs in:", build_dir)

    found_paths = list(build_dir.rglob(required))
    if not found_paths:
        print(f"❌Missing: {required} anywhere under {build_dir}")
        return False

    actual_dll_path = found_paths[0]
    print(f"Target discovered at: {actual_dll_path}")

    try:
        if os.name =='nt':
            os.add_dll_directory(str(actual_dll_path.parent))

        ctypes.CDLL(str(actual_dll_path))
        print(f"{required} - Loaded succesfully")
        return True
    except Exception as e:
        print(f"❌{required} - Failed to load: {str(e)}")
        return False

def configure_ctypes_signatures(raytracer, build_dir, vector3_class):
    """Safely hooks up C++ signatures to Python types with terminal reporting"""
    try:
        found_paths = list(build_dir.rglob("RaytracerCore.dll"))
        if found_paths:
           actual_dir = found_paths[0].parent
           if os.name == 'nt':
               os.add_dll_directory(str(actual_dir))
            
        raytracer.render_image.argtypes = [
            ctypes.c_char_p, 
            ctypes.c_char_p,
            ctypes.c_int,
            ctypes.c_int             
        ]
        
        if hasattr(raytracer, "setCamera"):
            raytracer.setCamera.argtypes = [
                ctypes.POINTER(vector3_class),
                ctypes.POINTER(vector3_class)
            ]
            print("🔗 C++ setCamera linked successfully.")
        else:
            print("⚠️ Warning: setCamera symbol not found in DLL wrapper boundary.")
            
        return raytracer
    except Exception as e:
        print(f"❌ Critical error mapping DLL signatures: {str(e)}")
        return None