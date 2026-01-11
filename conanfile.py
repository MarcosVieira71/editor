from conan import ConanFile
from conan.tools.cmake import cmake_layout

class EditorConan(ConanFile):
    name = "editor"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    requires = "stb/cci.20230920"

    generators = "CMakeToolchain", "CMakeDeps"

