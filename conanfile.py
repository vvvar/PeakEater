from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout

class PeakEater(ConanFile):
    name = "PeakEater"
    version = "0.6.2"
    user = "vvvar"
    channel = "testing"
    company = "T-Audio"
    url = "https://github.com/vvvar/PeakEater"
    settings = "os", "arch", "compiler", "build_type"

    requires = "juce/7.0.5@juce/release"
    default_options = {
        "juce/*:build_extras": True
    }

    def layout(self):
        cmake_layout(self)

    def generate(self): 
        toolchain = CMakeToolchain(self)
        if self.settings.os == "Windows":
            toolchain.generator = "Visual Studio 17 2022"
        else:
            toolchain.generator = "Ninja"
        if self.settings.os == "Macos":
            toolchain.cache_variables["CMAKE_OSX_ARCHITECTURES"] = "x86_64;arm64"
            toolchain.cache_variables["CMAKE_OSX_DEPLOYMENT_TARGET"] = "10.9"

        toolchain.cache_variables["CONAN_PROJECT_NAME"] = self.name
        toolchain.cache_variables["CONAN_PROJECT_COMPANY"] = self.company
        toolchain.cache_variables["CONAN_PROJECT_VERSION"] = self.version
        toolchain.cache_variables["CONAN_PROJECT_URL"] = self.url

        toolchain.generate()

        dependencies = CMakeDeps(self)
        dependencies.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
