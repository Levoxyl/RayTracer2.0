import os
import sys
import ctypes

def main():
    # Get the path dynamically
    base_dir = os.path.dirname(os.path.abspath(__file__))
    dll_path = os.path.join(base_dir, "..", "build", "libRaytracerCore.dll")
    
    try:
        # Add DLL directory to search path
        os.add_dll_directory(os.path.dirname(dll_path))
        
        # Load DLL
        raytracer = ctypes.CDLL(dll_path)
        print("DLL loaded successfully!")
        
        # Set function signature
        raytracer.render_image.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
        
        # Test function call
        print("Calling render_image...")
        raytracer.render_image(b"test.obj", 800, 600)
        print("Function called successfully!")
        
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()