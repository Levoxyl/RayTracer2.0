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
    try:
        raytracer.create_raytracer.argtypes = []
        raytracer.create_raytracer.restype = ctypes.c_void_p

        raytracer.destroy_raytracer.argtypes = [ctypes.c_void_p]
        raytracer.destroy_raytracer.restype = None

        raytracer.load_model_to_engine.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
        raytracer.load_model_to_engine.restype = None

        raytracer.set_engine_camera.argtypes = [
            ctypes.c_void_p, 
            ctypes.POINTER(vector3_class), 
            ctypes.POINTER(vector3_class)
        ]
        raytracer.set_engine_camera.restype = None

        raytracer.render_image.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
        raytracer.render_image.restype = None
        
        print("🔗 All stateful C++ engine signatures linked successfully.")
        return raytracer
    except Exception as e:
        print(f"❌ Critical error mapping DLL signatures: {str(e)}")
        return None