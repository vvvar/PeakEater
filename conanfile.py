from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import copy, rmdir, mkdir, rm
from conan.errors import ConanException, ConanInvalidConfiguration

import os


class PeakEater(ConanFile):
    name = "peakeater"
    version = "0.6.2"
    user = "vvvar"
    channel = "testing"
    company = "T-Audio"
    url = "https://github.com/vvvar/PeakEater"
    settings = "os", "arch", "compiler", "build_type"
    options = {"signed": [True, False]}
    default_options = {"signed": False}
    options_description = {"signed": "Whether binaries are signed with certificate or not"}
    exports_sources = {
        "*",
        "!.vscode/*",
        "!build/*",
        "!juce-conan/*",
        "!.env",
        "!.git*",
    }

    requires = "juce/7.0.5@juce/release"

    def validate(self):
        if self.options.signed and not self.settings.os == "Macos":  # type: ignore
            raise ConanInvalidConfiguration("Only macOS supports code-signing")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        toolchain = CMakeToolchain(self)
        toolchain.blocks["apple_system"].values["cmake_osx_architectures"] = "x86_64;arm64"
        # if self.settings.os == "Macos":  # type: ignore
        #     toolchain.cache_variables["CMAKE_OSX_ARCHITECTURES"] = "x86_64;arm64"
        #     toolchain.cache_variables["CMAKE_OSX_DEPLOYMENT_TARGET"] = "10.9"
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

        if self.options.signed and self.settings.os == "Macos":  # type: ignore
            self.output.info("Signing binaries...")
            identity = os.environ.get("MACOS_APPLE_IDENTITY")
            artefacts_folder = os.path.join(self.build_folder, f"{self.name}_artefacts", self.settings.get_safe("build_type"))  # type: ignore
            clap_folder = os.path.join(artefacts_folder, "CLAP")
            au_folder = os.path.join(artefacts_folder, "AU")
            vst3_folder = os.path.join(artefacts_folder, "VST3")
            lv2_folder = os.path.join(artefacts_folder, "LV2", f"{str(self.name)}.lv2")
            self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.clap --deep --strict --options=runtime --timestamp", cwd=clap_folder, quiet=True)
            self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.component --deep --strict --options=runtime --timestamp", cwd=au_folder, quiet=True)
            self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.vst3 --deep --strict --options=runtime --timestamp", cwd=vst3_folder, quiet=True)
            for filename in os.listdir(lv2_folder):  # LV2 plugin is a folder itself, so we need to sign all files from there
                self.run(f"codesign --force -s '{identity}' -v {filename} --deep --strict --options=runtime --timestamp", cwd=lv2_folder, quiet=True)
            self.output.success("Successfully signed binaries")

        if self.settings.os == "Macos":  # type: ignore
            self.output.info("Building DMG...")
            artefacts_folder = os.path.join(self.build_folder, f"{self.name}_artefacts", self.settings.get_safe("build_type"))  # type: ignore
            # Create DMG folder
            dmg_folder = os.path.join(artefacts_folder, "DMG")
            rmdir(self, dmg_folder)
            mkdir(self, dmg_folder)
            # Copy artefacts there
            copy(self, "*", src=os.path.join(artefacts_folder, "CLAP"), dst=dmg_folder)
            copy(self, "*", src=os.path.join(artefacts_folder, "AU"), dst=dmg_folder)
            copy(self, "*", src=os.path.join(artefacts_folder, "VST3"), dst=dmg_folder)
            copy(self, "*", src=os.path.join(artefacts_folder, "LV2"), dst=dmg_folder)
            copy(self, "appdmg-config.json", src=os.path.join(self.source_folder, "config"), dst=dmg_folder)  # type: ignore
            # Build DMG
            self.run("npm install -g appdmg")
            self.run(f"appdmg appdmg-config.json {str(self.name)}.dmg", cwd=dmg_folder)
            # When sign required
            if self.options.signed:  # type: ignore
                # Sign, notarize and staple DMG
                identity = os.environ.get("MACOS_APPLE_IDENTITY")
                apple_id = os.environ.get("MACOS_APPLE_ID")
                password = os.environ.get("MACOS_APPLE_PASSWORD")
                team_id = os.environ.get("MACOS_APPLE_TEAM_ID")
                self.run(f"codesign --force -s '{identity}' -v {str(self.name)}.dmg --deep --strict --options=runtime --timestamp", cwd=dmg_folder, quiet=True)
                self.run(f"xcrun notarytool submit {str(self.name)}.dmg --apple-id {apple_id} --password {password} --team-id {team_id} --wait", cwd=dmg_folder, quiet=True)
                self.run(f"xcrun stapler staple {str(self.name)}.dmg", cwd=dmg_folder, quiet=True)
            # Cleanup tmp files
            rm(self, ".json", dmg_folder)
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.clap"))
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.component"))
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.vst3"))
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.lv2"))
            self.output.success("Successfully built DMG")

    def package(self):
        raise ConanException(f"{str(self.name)} does not support Conan packaging")
