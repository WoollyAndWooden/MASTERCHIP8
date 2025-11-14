from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain


class MASTERCHIP8(ConanFile):
    name = "MASTERCHIP8"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "qtwayland": [True, False], "qtwebengine": [True, False]}
    default_options = {"shared": True, "qtwayland": False, "qtwebengine": True}  # default False
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = "*"

    def requirements(self):
        self.requires("portable-file-dialogs/0.1.0")

        if self.settings.os == "Linux":
            self.requires("qt/6.8.3", options={"shared":True, "qtwayland": True, "qt_feature_int128": True})
            self.requires("xorg/system")
            self.requires("opengl/system")
        else:
            self.requires("qt/6.8.3")

    def layout(self):
        cmake_layout(self)