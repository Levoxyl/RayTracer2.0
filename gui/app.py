import os
import sys
import subprocess
from pathlib import Path
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QPushButton, 
    QFileDialog, QLabel, QVBoxLayout, QWidget
)
from PyQt6.QtGui import QPixmap
from PyQt6.QtCore import Qt

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Raytracer")
        self.setGeometry(100, 100, 800, 600)
        
        layout = QVBoxLayout()
        
        self.button = QPushButton("Choose .obj file")
        self.button.clicked.connect(self.load_file)
        layout.addWidget(self.button)
        
        self.image_label = QLabel()
        self.image_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        layout.addWidget(self.image_label)
        
        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)
        
        self.current_file = None

    def load_file(self):
        path, _ = QFileDialog.getOpenFileName(
            self, 
            "Open 3D Model", 
            "", 
            "3D Files (*.obj *.fbx)"
        )
        if path:
            self.current_file = path
            self.render_image()
            
    def render_image(self):
        if not self.current_file:
            return
            
        # Find the executable
        base_path = Path(__file__).parent.parent
        exe_path = base_path / "build" / "RayTracer.exe"
        
        if not exe_path.exists():
            print(f"Error: Could not find RayTracer.exe at {exe_path}")
            return
        
        # Run the renderer
        try:
            result = subprocess.run(
                [str(exe_path), self.current_file, "800", "600"],
                capture_output=True,
                text=True,
                check=True
            )
            print("Rendering output:")
            print(result.stdout)
            if result.stderr:
                print("Errors:")
                print(result.stderr)
        except subprocess.CalledProcessError as e:
            print(f"Rendering failed: {e}")
            return
        
        # Display result
        output_path = base_path / "output.png"
        if output_path.exists():
            pixmap = QPixmap(str(output_path))
            if not pixmap.isNull():
                self.image_label.setPixmap(pixmap.scaled(
                    800, 600, Qt.AspectRatioMode.KeepAspectRatio
                ))
            else:
                print("Failed to load output image")
        else:
            print(f"Output image not found at {output_path}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())