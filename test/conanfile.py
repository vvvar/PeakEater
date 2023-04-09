import os
import pathlib
import json
import subprocess

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy, rmdir
from conan.api.output import ConanOutput, Color


def log_conan_stage(f):
    def wrapper(*args):
        message = f"Calling {f.__name__}()"
        ConanOutput().title(message)
        args[0].output.highlight(message)
        tmp = f(*args)
        args[0].output.success(f"[{f.__name__}] Success!")
        return tmp

    return wrapper


def mac_only(f):
    def wrapper(*args):
        if args[0].settings.os == "Macos":
            return f(*args)
        else:
            args[0].output.info("This test is intended for macOS only, skipping")

    return wrapper


class Test(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps"

    def requirements(self):
        self.tool_requires("pluginval/1.0.3@pluginval/release")  # type: ignore
        self.requires(self.tested_reference_str)  # type: ignore

    def build(self):
        pass

    def layout(self):
        self.cpp_info.set_property("tools.cmake.cmake_layout:build_folder_vars", ["settings.build_type"])
        cmake_layout(self)

    def test(self):
        dependency = self.dependencies[self.tested_reference_str]
        bin_dir = dependency.cpp_info.bindirs[0]
        res_dir = dependency.cpp_info.resdirs[0]
        self._log_dependency_under_test(dependency)
        self._test_compile_commands(res_dir)
        self._test_vst3(bin_dir)
        self._test_au(bin_dir)
        self._test_dmg(bin_dir)

    def _log_dependency_under_test(self, dependency):
        ConanOutput().title(f"Under test: {dependency.ref.name}")
        bin_dir = dependency.cpp_info.bindirs[0]
        res_dir = dependency.cpp_info.resdirs[0]
        self.output.highlight(f"Name: {dependency.ref.name}")
        self.output.highlight(f"Channel: {dependency.ref.channel}")
        self.output.highlight(f"Version: {dependency.ref.version}")
        self.output.highlight(f"Revision: {dependency.ref.revision}")
        self.output.highlight(f"Settings: {dependency.settings.serialize()}")
        self.output.highlight(f"Options: {dependency.options.serialize()}")
        self.output.highlight(f"Bin dir: {bin_dir}")
        self.output.highlight(f"Res dir: {res_dir}")
        self.output.highlight(f"Binaries: {os.listdir(bin_dir)}")
        self.output.highlight(f"Resources: {os.listdir(res_dir)}")

    @log_conan_stage
    def _test_compile_commands(self, res_dir):
        compile_commands = "".join(filter(lambda a: "compile_commands.json" in a, os.listdir(res_dir)))
        assert compile_commands, f"VST3 plugin not found in {res_dir}. What's actually inside: {str(os.listdir(res_dir))}"
        self._test_compile_commands_macos_universal_bins(os.path.join(res_dir, compile_commands))
        self._test_compile_commands_macos_backwards_compatibility(os.path.join(res_dir, compile_commands))

    @mac_only
    @log_conan_stage
    def _test_compile_commands_macos_universal_bins(self, compile_commands_path):
        if self.settings.os == "Macos":  # type: ignore
            with open(compile_commands_path, encoding="utf-8") as f:
                compile_commands = json.load(f)
                for compilation_unit in compile_commands:
                    assert "-arch x86_64" in compilation_unit["command"]
                    assert "-arch arm64" in compilation_unit["command"]
        else:
            self.output.info("Non-macOS host, skipping")

    @mac_only
    @log_conan_stage
    def _test_compile_commands_macos_backwards_compatibility(self, compile_commands_path):
        if self.settings.os == "Macos":  # type: ignore
            with open(compile_commands_path, encoding="utf-8") as f:
                compile_commands = json.load(f)
                for compilation_unit in compile_commands:
                    assert "-mmacosx-version-min=10.9" in compilation_unit["command"]
        else:
            self.output.info("Non-macOS host, skipping")

    @log_conan_stage
    def _test_vst3(self, bin_dir):
        """Test VST3 Plugin with pluginval"""
        vst3 = "".join(filter(lambda a: ".vst3" in a, os.listdir(bin_dir)))
        assert vst3, f"VST3 plugin not found in {bin_dir}. What's actually inside: {str(os.listdir(bin_dir))}"
        self._pluginval(os.path.join(bin_dir, vst3))

    @mac_only
    @log_conan_stage
    def _test_au(self, bin_dir):
        """
        Test AU(.component) Plugin with pluginval
        NOTE: Only relevant for macOS
        NOTE: macOS requires AU plugins to be registered in order to be discoverable
        NOTE: Registration means it is placed either in system or in user's AU folder
        NOTE: Discussion - https://github.com/Tracktion/pluginval/issues/39
        """
        # Get AU plugin
        au_plugin_name = "".join(filter(lambda a: ".component" in a, os.listdir(bin_dir)))
        assert au_plugin_name, f"AU plugin not found in {bin_dir}. What's actually inside: {str(os.listdir(bin_dir))}"
        # Copy it to user plugins folder(~/Library/Audio/Plug-Ins/Components)
        user_au_plugins = os.path.join(pathlib.Path.home(), "Library", "Audio", "Plug-Ins", "Components")
        rmdir(self, os.path.join(user_au_plugins, au_plugin_name))
        copy(self, "*", src=os.path.join(bin_dir, au_plugin_name), dst=os.path.join(user_au_plugins, au_plugin_name))
        self.run(f"file {os.path.join(user_au_plugins, au_plugin_name)}")  # Verify it is copied
        # Trigger AudioComponentRegistrar and auval, this will force macOS to register it
        self.run("killall -9 AudioComponentRegistrar", ignore_errors=True)
        self.run("auval -a", ignore_errors=True)
        # Finally, test it from user plugin folder
        self._pluginval(os.path.join(user_au_plugins, au_plugin_name))
        # Cleanup after the test
        rmdir(self, os.path.join(user_au_plugins, au_plugin_name))

    def _pluginval(self, plugin):
        # Max level
        strictness = 10
        # GUI does not work on a CI(except Windows)
        skip_gui_test = "--skip-gui-tests" if self.settings.os in ["Macos", "Linux"] else ""  # type: ignore
        output_dir = os.path.join(self.build_folder, "pluginval")  # type: ignore
        self.run(f"pluginval --strictness-level {strictness} --verbose {skip_gui_test} --validate-in-process --output-dir {output_dir} --validate {plugin}")

    @mac_only
    @log_conan_stage
    def _test_dmg(self, bin_dir):
        dmg = "".join(filter(lambda a: ".dmg" in a, os.listdir(bin_dir)))
        assert dmg, f"DMG image not found in {bin_dir}. What's actually inside: {str(os.listdir(bin_dir))}"
        process = subprocess.Popen(["codesign", "-dv", "--verify", "--verbose=4", os.path.join(bin_dir, dmg)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (_, stderr) = process.communicate()
        assert 0 == process.returncode, f"codesign returned non-zero exit code: {process.returncode}. Stderr: {stderr.decode()}"
        assert "valid on disk" in stderr.decode(), f"DMG file {os.path.join(bin_dir, dmg)} is not valid"
        assert "satisfies its Designated Requirement" in stderr.decode(), f"DMG file {os.path.join(bin_dir, dmg)} has invalid signature"
