from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain


class MASTERCHIP8(ConanFile):
    name = "MASTERCHIP8"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "qtwayland": [True, False], "qtwebengine": [True, False]}
    default_options = {"shared": True, "qtwayland": False, "qtwebengine": True}  # default False
    generators = "CMakeDeps"
    exports_sources = "*"

    def config_options(self):
        if self.settings.os == "Linux":
            self.options.qtwayland = True
            self.options.qtwebengine = False
        else:
            self.options.qtwayland = False  # ensure disabled for Windows/macOS

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["QT_FEATURE_int128"] = "ON"  # set INT128 support
        tc.generate()

    def requirements(self):
        self.requires("qt/6.8.3")
        self.requires("portable-file-dialogs/0.1.0")

        if self.settings.os == "Linux":
            self.requires("xorg/system")
            self.requires("opengl/system")

    def layout(self):
        cmake_layout(self)