import unittest
import os
import sys
import json


def isMac():
    return sys.platform == "darwin"


class BuildArtefactsTestSuite(unittest.TestCase):
    """Set of tests to check result of the build"""

    compile_commands_path = os.path.abspath(os.path.join(__file__, "../..", "build", "Release", "compile_commands.json"))

    def test_compile_commands_exist(self):
        """Make sure compile_commands.json is generated"""
        self.assertTrue(os.path.exists(self.compile_commands_path))

    def test_compile_commands_not_corrupted(self):
        """Make sure compile_commands.json is not corrupted"""
        with open(self.compile_commands_path, encoding="utf-8") as f:
            json.load(f)

    @unittest.skipUnless(isMac(), "Only relevant for macOS")
    def test_compile_commands_universal_binary(self):
        """Make sure every compilation unit from compile_commands.json compiled with support of both x86_64 and arm64"""
        with open(self.compile_commands_path, encoding="utf-8") as f:
            compile_commands = json.load(f)
            for compilation_unit in compile_commands:
                self.assertIn("-arch x86_64", compilation_unit["command"])
                self.assertIn("-arch arm64", compilation_unit["command"])

    @unittest.skipUnless(isMac(), "Only relevant for macOS")
    def test_compile_commands_backwards_compatible(self):
        """Make sure every compilation unit from compile_commands.json compiled with macOS 10.9+ support"""
        with open(self.compile_commands_path, encoding="utf-8") as f:
            compile_commands = json.load(f)
            for compilation_unit in compile_commands:
                self.assertIn("-mmacosx-version-min=10.9", compilation_unit["command"])


if __name__ == "__main__":
    unittest.main()
