from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.system.package_manager import Chocolatey, Brew, Apt
from conan.tools.files import copy
from conan.errors import ConanInvalidConfiguration

import os

class PeakEater(ConanFile):
    name = "peakeater"
    version = "0.6.2"
    user = "vvvar"
    channel = "testing"
    company = "T-Audio"
    url = "https://github.com/vvvar/PeakEater"
    settings = "os", "arch", "compiler", "build_type"
    options = { "signed": [True, False] }
    default_options = { "signed": False }
    options_description = {
        "signed": "Whether binaries are signed with certificate or not"
    }

    requires = "juce/7.0.5@juce/release"

    def validate(self):
        if self.options.signed and not self.settings.os == "Macos":
            raise ConanInvalidConfiguration("Only macOS supports code-signing")

    def layout(self):
        cmake_layout(self)

    def generate(self): 
        toolchain = CMakeToolchain(self, generator="Ninja")
        if self.settings.os == "Macos":
            toolchain.cache_variables["CMAKE_OSX_ARCHITECTURES"] = "x86_64;arm64"
            toolchain.cache_variables["CMAKE_OSX_DEPLOYMENT_TARGET"] = "10.9"
        toolchain.cache_variables["CONAN_PROJECT_NAME"] = str(self.name)
        toolchain.cache_variables["CONAN_PROJECT_COMPANY"] = str(self.company)
        toolchain.cache_variables["CONAN_PROJECT_VERSION"] = str(self.version)
        toolchain.cache_variables["CONAN_PROJECT_URL"] = str(self.url)
        toolchain.generate()
        dependencies = CMakeDeps(self)
        dependencies.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package_info(self):
        self.cpp_info.libs = [
            f"{str(self.name)}.clap",
            f"{str(self.name)}.lv2",
            f"{str(self.name)}.vst3"
        ]
        if self.settings.os == "Macos":
            self.cpp_info.libs.append(f"{str(self.name)}.component")

    def package(self):
        artefacts_folder = os.path.join(self.build_folder, f"{self.name}_artefacts", self.settings.get_safe("build_type"))
        libdir = os.path.join(self.package_folder, self.cpp.package.libdirs[0])
        copy(self, "*", src=os.path.join(artefacts_folder, "CLAP"), dst=libdir)
        copy(self, "*", src=os.path.join(artefacts_folder, "LV2"), dst=libdir)
        copy(self, "*", src=os.path.join(artefacts_folder, "VST3"), dst=libdir)

        if self.settings.os == "Macos":
            copy(self, "*", src=os.path.join(artefacts_folder, "AU"), dst=libdir)

            if self.options.signed:
                self.output.info("Signing binaries...")
                identity = os.environ.get('MACOS_APPLE_IDENTITY')
                self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.clap --deep --strict --options=runtime --timestamp", cwd=libdir, quiet=True)
                self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.component --deep --strict --options=runtime --timestamp", cwd=libdir, quiet=True)
                self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.vst3 --deep --strict --options=runtime --timestamp", cwd=libdir, quiet=True)
                for filename in os.listdir(os.path.join(libdir, f"{str(self.name)}.lv2")): # LV2 is a directory, we need to sign all files from there
                    self.run(f"codesign --force -s '{identity}' -v {filename} --deep --strict --options=runtime --timestamp", cwd=os.path.join(libdir, f"{str(self.name)}.lv2"), quiet=True)
                self.output.success("Successfully signed binaries")

            self.run("npm install -g appdmg")
            copy(self, "appdmg-config.json", src=os.path.join(self.source_folder, "Scripts", "Release", "configs"), dst=libdir)
            self.run(f"appdmg {os.path.join(libdir, 'appdmg-config.json')} {os.path.join(libdir, str(self.name))}.dmg")
            
            if self.options.signed:
                identity = os.environ.get('MACOS_APPLE_IDENTITY')
                apple_id = os.environ.get('MACOS_APPLE_ID')
                password = os.environ.get('MACOS_APPLE_PASSWORD')
                team_id = os.environ.get('MACOS_APPLE_TEAM_ID')
                self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.dmg --deep --strict --options=runtime --timestamp", cwd=libdir, quiet=True)
                self.run(f"xcrun notarytool submit {str(self.name)}.dmg --apple-id {apple_id} --password {password} --team-id {team_id} --wait", cwd=libdir, quiet=True)
                self.run(f"xcrun stapler staple {str(self.name)}.dmg", cwd=libdir, quiet=True)
                