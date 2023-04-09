from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import copy, rmdir, mkdir, rm
from conan.errors import ConanException, ConanInvalidConfiguration
from conan.tools.env import VirtualBuildEnv
from conan.api.output import ConanOutput, Color

import os


def log_conan_stage(f):
    def wrapper(*args):
        message = f"Calling {f.__name__}()"
        ConanOutput().title(message)
        args[0].output.highlight(message)
        return f(*args)

    return wrapper


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
        toolchain.blocks.remove("apple_system")  # Because Conan forces x86_64 build(from settings)
        toolchain.cache_variables["CMAKE_OSX_ARCHITECTURES"] = "x86_64;arm64"
        toolchain.cache_variables["CMAKE_OSX_DEPLOYMENT_TARGET"] = "10.9"
        toolchain.cache_variables["CONAN_PROJECT_NAME"] = str(self.name)
        toolchain.cache_variables["CONAN_PROJECT_COMPANY"] = str(self.company)
        toolchain.cache_variables["CONAN_PROJECT_VERSION"] = str(self.version)
        toolchain.cache_variables["CONAN_PROJECT_URL"] = str(self.url)
        toolchain.generate()
        dependencies = CMakeDeps(self)
        dependencies.generate()
        buildenv = VirtualBuildEnv(self)
        buildenv.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        self._sign()
        self._bundle()

    @log_conan_stage
    def _sign(self):
        if self.options.signed and self.settings.os == "Macos":  # type: ignore
            identity = os.environ.get("MACOS_APPLE_IDENTITY")
            artefacts_folder = os.path.join(self.build_folder, f"{self.name}_artefacts", self.settings.get_safe("build_type"))  # type: ignore
            self._codesign_file(f"{str(self.name)}.clap", os.path.join(artefacts_folder, "CLAP"), identity)
            self._codesign_file(f"{str(self.name)}.component", os.path.join(artefacts_folder, "AU"), identity)
            self._codesign_file(f"{str(self.name)}.vst3", os.path.join(artefacts_folder, "VST3"), identity)
            # LV2 plugin is a folder itself, so we need to sign all files from there
            lv2_folder = os.path.join(artefacts_folder, "LV2", f"{str(self.name)}.lv2")
            for filename in os.listdir(lv2_folder):
                self._codesign_file(filename, lv2_folder, identity)
            self.output.success("Successfully signed binaries!")
        else:
            self.output.warning("Signing is not supported, skipped")

    @log_conan_stage
    def _bundle(self):
        if self.settings.os == "Macos":  # type: ignore
            self.output.info("Building DMG...")
            # Create DMG folder
            self.output.info("Creating the folder to store DMG...")
            artefacts_folder = os.path.join(self.build_folder, f"{self.name}_artefacts", self.settings.get_safe("build_type"))  # type: ignore
            dmg_folder = os.path.join(artefacts_folder, "DMG")
            rmdir(self, dmg_folder)
            mkdir(self, dmg_folder)
            # Copy artefacts there
            self.output.info("Copying artefacts to the DMG folder...")
            copy(self, "*", src=os.path.join(artefacts_folder, "CLAP"), dst=dmg_folder)
            copy(self, "*", src=os.path.join(artefacts_folder, "AU"), dst=dmg_folder)
            copy(self, "*", src=os.path.join(artefacts_folder, "VST3"), dst=dmg_folder)
            copy(self, "*", src=os.path.join(artefacts_folder, "LV2"), dst=dmg_folder)
            copy(self, "appdmg.json", src=os.path.join(self.source_folder, "config"), dst=dmg_folder)  # type: ignore
            # Build DMG
            self.output.info("Building DMG...")
            self.run("npm install -g appdmg")
            self.run(f"appdmg appdmg.json {str(self.name)}.dmg", cwd=dmg_folder)
            # When sign required
            if self.options.signed:  # type: ignore
                # Sign DMG
                self.output.info("Signing DMG...")
                self._codesign_file(f"{str(self.name)}.dmg", dmg_folder, os.environ.get("MACOS_APPLE_IDENTITY"))
                # Notarize and staple
                self.output.info("Notarizing DMG...")
                self._notarize_file(
                    filename=f"{str(self.name)}.dmg",
                    cwd=dmg_folder,
                    apple_id=os.environ.get("MACOS_APPLE_ID"),
                    password=os.environ.get("MACOS_APPLE_PASSWORD"),
                    team_id=os.environ.get("MACOS_APPLE_TEAM_ID"),
                )
            # Cleanup tmp files
            self.output.info("Cleaning up tmp files...")
            rm(self, "*.json", dmg_folder)
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.clap"))
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.component"))
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.vst3"))
            rmdir(self, os.path.join(dmg_folder, f"{str(self.name)}.lv2"))
            self.output.success("Successfully built DMG!")
        else:
            self.output.warning("Bundling is not supported, skipped")

    def _codesign_file(self, filename, cwd, identity):
        self.output.info(f"Code-signing file {filename} in {cwd}...", bg=Color.BRIGHT_BLUE)
        self.run(f"codesign --force -s '{identity}' -v {filename} --deep --strict --options=runtime --timestamp", cwd=cwd, quiet=True)
        self.output.success(f"Successfully code-signed {filename}!")

    def _notarize_file(self, filename, cwd, apple_id, password, team_id):
        self.output.info(f"Notarizing file {filename} in {cwd}...", bg=Color.BLUE)
        self.run(f"xcrun notarytool submit {filename} --apple-id {apple_id} --password {password} --team-id {team_id} --wait", cwd=cwd, quiet=True)
        self.run(f"xcrun stapler staple {filename}", cwd=cwd, quiet=True)
        self.output.success(f"Successfully notarized {filename}!")

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
