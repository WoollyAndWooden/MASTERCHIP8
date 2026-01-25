from conan import ConanFile
from conan.tools.cmake import cmake_layout


class MASTERCHIP8(ConanFile):
    name = "MASTERCHIP8"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}  # default False
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = "*"

    def requirements(self):
        self.requires("portable-file-dialogs/0.1.0")
        self.requires("gtest/1.14.0")
        self.requires("pybind11/2.11.1")

        if self.settings.os == "Linux":
            self.requires("xorg/system")
            self.requires("opengl/system")

    def layout(self):
        cmake_layout(self)
