import os
import sys
import ctypes
from pathlib import Path
from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QPushButton, 
    QFileDialog, QLabel, QVBoxLayout, QWidget,
    QHBoxLayout, QLineEdit, QScrollArea, QGridLayout,
    QDoubleSpinBox
)
from PyQt6.QtGui import QPixmap
from PyQt6.QtCore import Qt

# Import your safety framework
import safetyNet

class Vector3(ctypes.Structure):
    _fields_ = [
        ("x", ctypes.c_float),
        ("y", ctypes.c_float),
        ("z", ctypes.c_float)
    ]

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Raytracer Workspace")
        self.setGeometry(100, 100, 1100, 700)

        def closeEvent(self, event):
            if self.raytracer_dll and self.engine_ptr:
                self.raytracer_dll.destroy_raytracer(self.engine_ptr)
            super().closeEvent(event)

        #ptr token
        self.raytracer_dll = self.load_dll_pipeline()
        self.engine_ptr = None
        if self.raytracer_dll and hasattr(self.raytracer_dll, "create_raytracer"):
            self.engine_ptr = self.raytracer_dll.create_raytracer()
        
        self.setWindowFlag(Qt.WindowType.WindowMaximizeButtonHint, True)
        self.setWindowFlag(Qt.WindowType.WindowMinimizeButtonHint, True)
        
        self.defaults = {
            "pos_x": 0.0, "pos_y": 1.5, "pos_z": 4.0,
            "look_x": 0.0, "look_y": 0.0, "look_z": 0.0
        }
        
        self.current_pixmap = None
        self.is_updating = False
        
        # Load DLL using our safety asset module
        self.raytracer = self.load_dll_pipeline()
        
        # UI Assembly
        main_layout = QVBoxLayout()
        
        file_layout = QHBoxLayout()
        self.model_button = QPushButton("Choose 3D Model")
        self.model_button.clicked.connect(self.select_model)
        self.model_path = QLineEdit()
        self.model_path.setReadOnly(True)
        self.model_path.setPlaceholderText("No model selected")
        file_layout.addWidget(self.model_button)
        file_layout.addWidget(self.model_path)
        
        output_layout = QHBoxLayout()
        self.output_button = QPushButton("Output Location")
        self.output_button.clicked.connect(self.select_output)
        self.output_path = QLineEdit()
        self.output_path.setText(str(Path.home() / "Downloads" / "output.png"))
        self.output_path.setReadOnly(True)
        output_layout.addWidget(self.output_button)
        output_layout.addWidget(self.output_path)
        
        self.render_button = QPushButton("Production Render (High Quality)")
        self.render_button.clicked.connect(self.render_production)
        self.render_button.setEnabled(False)
        
        main_layout.addLayout(file_layout)
        main_layout.addLayout(output_layout)
        main_layout.addWidget(self.render_button)
        
        workspace_layout = QHBoxLayout()
        
        control_panel = QWidget()
        control_panel.setFixedWidth(320)
        grid_layout = QGridLayout(control_panel)
        grid_layout.setSpacing(10)
        
        self.inputs = {}
        coords = [
            ("pos_x", "Camera X", 0, 0, self.defaults["pos_x"]),
            ("pos_y", "Camera Y", 0, 1, self.defaults["pos_y"]),
            ("pos_z", "Camera Z", 0, 2, self.defaults["pos_z"]),
            ("look_x", "LookAt X", 1, 0, self.defaults["look_x"]),
            ("look_y", "LookAt Y", 1, 1, self.defaults["look_y"]),
            ("look_z", "LookAt Z", 1, 2, self.defaults["look_z"])
        ]
        
        for key, label_text, row, col, default_val in coords:
            cell_widget = QWidget()
            cell_layout = QVBoxLayout(cell_widget)
            cell_layout.setContentsMargins(2, 2, 2, 2)
            
            label = QLabel(label_text)
            label.setStyleSheet("font-weight: bold;")
            
            spin = QDoubleSpinBox()
            spin.setRange(-50.0, 50.0)
            spin.setSingleStep(0.1)
            spin.setDecimals(2)
            spin.setValue(default_val)
            spin.valueChanged.connect(self.on_camera_modified)
            
            reset_btn = QPushButton("Reset")
            reset_btn.setFixedHeight(20)
            reset_btn.setStyleSheet("font-size: 10px;")
            reset_btn.clicked.connect(lambda checked, s=spin, d=default_val: s.setValue(d))
            
            cell_layout.addWidget(label)
            cell_layout.addWidget(spin)
            cell_layout.addWidget(reset_btn)
            
            grid_layout.addWidget(cell_widget, row, col)
            self.inputs[key] = spin
            
        workspace_layout.addWidget(control_panel)
        
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.image_label = QLabel("Load a model to initialize workspace viewport")
        self.image_label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.image_label.setStyleSheet("background-color: #202020; color: #808080;")
        self.scroll_area.setWidget(self.image_label)
        
        workspace_layout.addWidget(self.scroll_area)
        main_layout.addLayout(workspace_layout)
        
        container = QWidget()
        container.setLayout(main_layout)
        self.setCentralWidget(container)

    def load_dll_pipeline(self):
        build_dir = Path(__file__).parent.parent / "out" / "build" / "x64-Debug"
        dll_path = build_dir / "RaytracerCore.dll"
        
        if not safetyNet.verify_dlls():
            print("❌ Initialization halted: Verification checks failed.")
            return None
            
        try:
            raytracer = ctypes.CDLL(str(dll_path))
            return safetyNet.configure_ctypes_signatures(raytracer, build_dir, Vector3)
        except Exception as e:
            print(f"❌ Critical error loading DLL during startup step: {str(e)}")
            return None
            
    def select_model(self):
        path, _ = QFileDialog.getOpenFileName(
            self, "Open 3D Model", "", "OBJ Files (*.obj);;All Files (*)"
        )
        if path:
            self.model_path.setText(path)
            self.render_button.setEnabled(True)
            print(f"📦 Model path loaded: {path}")
            if self.raytracer_dll and self.engine_ptr:
                self.raytracer_dll.load_model_to_engine(self.engine_ptr, path.encode('utf-8'))
            
    def select_output(self):
        path, _ = QFileDialog.getSaveFileName(
            self, "Save Rendered Image As", self.output_path.text(), "PNG Images (*.png);;All Files (*)"
        )
        if path:
            if not path.lower().endswith('.png'):
                path += '.png'
            self.output_path.setText(path)

    def on_camera_modified(self):
        if self.is_updating or not self.model_path.text() or not self.raytracer:
            return
            
        self.is_updating = True
        
        pos = Vector3(self.inputs["pos_x"].value(), self.inputs["pos_y"].value(), self.inputs["pos_z"].value())
        target = Vector3(self.inputs["look_x"].value(), self.inputs["look_y"].value(), self.inputs["look_z"].value())
        
        if hasattr(self.raytracer_dll, "set_engine_camera") and self.engine_ptr:
            self.raytracer_dll.set_engine_camera(self.engine_ptr, ctypes.byref(pos), ctypes.byref(target))
            
        self.trigger_render(width=200, height=150)
        self.is_updating = False

    def render_production(self):
        print("🚀 Launching full production rendering pass...")
        self.trigger_render(width=800, height=600)

    def trigger_render(self, width, height):
        if not self.model_path.text() or not self.output_path.text():
            print("⚠️ Cannot render: Missing model path or output path setup.")
            return
            
        if not self.raytracer:
            print("❌ Cannot render: Raytracer DLL core engine is completely offline.")
            return
            
        try:
            self.raytracer_dll.render_image(
                self.engine_ptr,
                self.output_path.text().encode('utf-8'),
                width, 
                height
            )
            print(f"✨ Frame Render Complete! Dimensions: {width}x{height}", flush=True)
            
            if os.path.exists(self.output_path.text()):
                self.current_pixmap = QPixmap(self.output_path.text())
                if not self.current_pixmap.isNull():
                    self.update_image_display()
                else:
                    print("❌ Image Check Failure: Output file is null or corrupted format.")
            else:
                print(f"❌ IO Failure: Output target missing from disk at {self.output_path.text()}")
        except Exception as e:
            print(f"💥 Native rendering runtime exception encountered: {e}")
            
    def update_image_display(self):
        if not self.current_pixmap or self.current_pixmap.isNull(): 
            return 
            
        available_width = self.scroll_area.viewport().width()
        available_height = self.scroll_area.viewport().height()
        
        scaled = self.current_pixmap.scaled(
            available_width,
            available_height,
            Qt.AspectRatioMode.KeepAspectRatio,
            Qt.TransformationMode.FastTransformation if self.is_updating else Qt.TransformationMode.SmoothTransformation
        )
        self.image_label.setPixmap(scaled)
        
    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.update_image_display()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())