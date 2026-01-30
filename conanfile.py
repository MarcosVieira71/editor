from conan import ConanFile
from conan.tools.cmake import cmake_layout

class EditorConan(ConanFile):
    name = "editor"
    version = "0.1"

    settings = "os", "compiler", "build_type", "arch"

    requires = [
        "stb/cci.20230920",
        "qt/5.15.14",
        "llvm-openmp/20.1.6"
    ]

    generators = (
        "CMakeToolchain",
        "CMakeDeps",
    )

    default_options = {
        "qt/*:shared": True,
        "qt/*:with_xcb": True,
        "qt/*:qttools": True
    }
