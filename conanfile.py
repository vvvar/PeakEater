from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import copy
from conan.tools.env import VirtualBuildEnv

import os


class PeakEater(ConanFile):
    name = "peakeater"
    version = "0.6.3"
    user = "vvvar"
    channel = "testing"
    company = "T-Audio"
    url = "https://github.com/vvvar/PeakEater"
    settings = "os", "arch", "compiler", "build_type"
    options = {"signed": [True, False]}
    default_options = {"signed": False}
    options_description = {"signed": "Whether binaries are signed with the certificate or not"}
    exports_sources = "assets", "modules", "source", "CMakeLists.txt"
    package_type = "application"

    requires = "juce/7.0.5@juce/release"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.blocks.remove("apple_system")  # Because Conan forces x86_64 build(from settings)
        toolchain.cache_variables["CMAKE_PROJECT_NAME"] = str(self.name)
        toolchain.cache_variables["CMAKE_PROJECT_VERSION"] = str(self.version)
        toolchain.cache_variables["CONAN_PROJECT_COMPANY"] = str(self.company)
        toolchain.cache_variables["CONAN_PROJECT_URL"] = str(self.url)
        toolchain.cache_variables["CODESIGN"] = self.options.signed  # type: ignore
        toolchain.generate()
        dependencies = CMakeDeps(self)
        dependencies.generate()
        buildenv = VirtualBuildEnv(self)
        buildenv.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package_info(self):
        self.cpp_info.resdirs = ["data"]

    def package(self):
        # Package bins(plugins, installers, etc.)
        bin_folder = os.path.join(str(self.package_folder), "bin")
        artefacts_folder = os.path.join(self.build_folder, f"{self.name}_artefacts", self.settings.get_safe("build_type"))  # type: ignore
        copy(self, "*", src=os.path.join(artefacts_folder, "VST3"), dst=bin_folder)
        copy(self, "*", src=os.path.join(artefacts_folder, "CLAP"), dst=bin_folder)
        copy(self, "*", src=os.path.join(artefacts_folder, "LV2"), dst=bin_folder)
        if self.settings.os == "Macos":  # type: ignore
            copy(self, "*", src=os.path.join(artefacts_folder, "AU"), dst=bin_folder)
            copy(self, "*", src=os.path.join(artefacts_folder, "DMG"), dst=bin_folder)
        # Package various build data(build logs, compile database, etc). Useful for analysis
        data_folder = os.path.join(str(self.package_folder), "data")
        copy(self, "compile_commands.json", src=self.build_folder, dst=data_folder)
        copy(self, "build.ninja", src=self.build_folder, dst=data_folder)
