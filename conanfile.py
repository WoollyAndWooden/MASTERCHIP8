from conan import ConanFile

class MyProjectConan(ConanFile):
    name = "MASTERCHIP8"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "qtwayland": [True, False]}
    default_options = {"shared": True, "qtwayland": False}  # default False
    generators = "CMakeDeps", "CMakeToolchain"
    exports_sources = "*"

    def config_options(self):
        if self.settings.os == "Linux":
            self.options.qtwayland = True
        else:
            self.options.qtwayland = False  # ensure disabled for Windows/macOS

    def requirements(self):
        self.requires("qt/6.8.3")
        self.requires("portable-file-dialogs/0.1.0")

        if self.settings.os == "Linux":
            self.requires("xorg/system")
            self.requires("opengl/system")

    def layout(self):
        self.folders.build = "build"
        self.folders.source = "."