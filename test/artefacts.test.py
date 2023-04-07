import unittest
import subprocess
import os
import sys
import json


def isMac():
    return sys.platform == "darwin"


class TestBuildArtefacts(unittest.TestCase):
    """Make sure build artefacts went produced as expected"""

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

    @unittest.skipUnless(isMac(), "Only relevant for macOS")
    def test_dmg_signed(self):
        """Make sure DMG file has been signed"""
        dmg_path = os.path.abspath(os.path.join(__file__, "../..", "build", "Release", "peakeater_artefacts", "Release", "DMG", "peakeater.dmg"))
        process = subprocess.Popen(["codesign", "-dv", "--verify", "--verbose=4", dmg_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        (_, stderr) = process.communicate()
        self.assertEqual(0, process.returncode)
        # Because of bug in codesign utility, it returns report into stdout
        self.assertIn("valid on disk", stderr.decode())
        self.assertIn("satisfies its Designated Requirement", stderr.decode())


if __name__ == "__main__":
    unittest.main(verbosity=2)
