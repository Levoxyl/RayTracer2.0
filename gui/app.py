import os
import sys
import ctypes
from pathlib import Path
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QPushButton, 
    QFileDialog, QLabel, QVBoxLayout, QWidget,
    QHBoxLayout, QLineEdit, QScrollArea
)
from PyQt6.QtGui import QPixmap
from PyQt6.QtCore import Qt

# Add the verify_dlls function here (before the MainWindow class)
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

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Raytracer")
        self.setGeometry(100, 100, 800, 600)
        
        # Make window resizable
        self.setWindowFlag(Qt.WindowType.WindowMaximizeButtonHint, True)
        self.setWindowFlag(Qt.WindowType.WindowMinimizeButtonHint, True)
        
        main_layout = QVBoxLayout()
        
        # File selection layout
        file_layout = QHBoxLayout()
        self.model_button = QPushButton("Choose 3D Model")
        self.model_button.clicked.connect(self.select_model)
        self.model_path = QLineEdit()
        self.model_path.setReadOnly(True)
        self.model_path.setPlaceholderText("No model selected")
        file_layout.addWidget(self.model_button)
        file_layout.addWidget(self.model_path)
        
        # Output selection layout
        output_layout = QHBoxLayout()
        self.output_button = QPushButton("Output Location")
        self.output_button.clicked.connect(self.select_output)
        self.output_path = QLineEdit()
        self.output_path.setText(str(Path.home() / "Downloads" / "output.png"))
        self.output_path.setReadOnly(True)
        output_layout.addWidget(self.output_button)
        output_layout.addWidget(self.output_path)
        
        # Render button
        self.render_button = QPushButton("Render")
        self.render_button.clicked.connect(self.render_image)
        self.render_button.setEnabled(False)
        
        # Image display with scroll area
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.image_label = QLabel()
        self.image_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.scroll_area.setWidget(self.image_label)
        
        # Add to main layout
        main_layout.addLayout(file_layout)
        main_layout.addLayout(output_layout)
        main_layout.addWidget(self.render_button)
        main_layout.addWidget(self.scroll_area)
        
        container = QWidget()
        container.setLayout(main_layout)
        self.setCentralWidget(container)
        
        # Load DLL
        self.raytracer = self.load_dll()
        
        # Store current pixmap for resize handling
        self.current_pixmap = None
        
    def load_dll(self):
        build_dir = Path(__file__).parent.parent / "build"
        dll_path = build_dir / "libRaytracerCore.dll"
        
        # Verify dependencies first
        if not verify_dlls():  # Reuse the verification function
            return None
            
        try:
            # Windows-specific loading
            if os.name == 'nt':
                os.add_dll_directory(str(build_dir))
                os.add_dll_directory("C:\\mingw64\\bin")
                
            raytracer = ctypes.CDLL(str(dll_path))
            raytracer.render_image.argtypes = [
                ctypes.c_char_p, 
                ctypes.c_char_p,
                ctypes.c_int,
                ctypes.c_int
            ]
            return raytracer
        except Exception as e:
            print(f"Critical error loading DLL: {str(e)}")
            return None
            
    def select_model(self):
        path, _ = QFileDialog.getOpenFileName(
            self, 
            "Open 3D Model", 
            "", 
            "OBJ Files (*.obj);;All Files (*)"
        )
        if path:
            self.model_path.setText(path)
            self.render_button.setEnabled(True)
            
    def select_output(self):
        path, _ = QFileDialog.getSaveFileName(
            self,
            "Save Rendered Image",
            self.output_path.text(),
            "PNG Images (*.png);;All Files (*)"
        )
        if path:
            if not path.lower().endswith('.png'):
                path += '.png'
            self.output_path.setText(path)
    
    def render_image(self):
        if not self.model_path.text() or not self.output_path.text():
            return
            
        if not self.raytracer:
            print("Renderer not loaded!")
            return
            
        try:
            # Call C++ renderer
            self.raytracer.render_image(
                self.model_path.text().encode('utf-8'),
                self.output_path.text().encode('utf-8'),
                800, 
                600
            )
            print("Rendering complete!")
        except Exception as e:
            print(f"Rendering failed: {e}")
            return
        
        # Display result
        if os.path.exists(self.output_path.text()):
            self.current_pixmap = QPixmap(self.output_path.text())
            if not self.current_pixmap.isNull():
                self.update_image_display()
            else:
                print("Failed to load output image")
        else:
            print(f"Output image not found at {self.output_path.text()}")
    
    def update_image_display(self):
        """Update image display with proper scaling"""
        if not self.current_pixmap or self.current_pixmap.isNull():
            return
            
        # Get available space in scroll area
        available_width = self.scroll_area.viewport().width()
        available_height = self.scroll_area.viewport().height()
        
        # Scale pixmap while preserving aspect ratio
        scaled = self.current_pixmap.scaled(
            available_width,
            available_height,
            Qt.AspectRatioMode.KeepAspectRatio,
            Qt.TransformationMode.SmoothTransformation
        )
        self.image_label.setPixmap(scaled)
    
    def resizeEvent(self, event):
        """Handle window resize events"""
        super().resizeEvent(event)
        self.update_image_display()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())