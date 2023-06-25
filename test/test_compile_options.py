import unittest
import sys
import argparse
import json

global compile_commands_path

IS_MACOS = sys.platform == "darwin"


class TestCompileOptions(unittest.TestCase):
    @unittest.skipUnless(IS_MACOS, "Only relevant for macOS")
    def test_macos_min_version(self):
        """macOS establishes backwards compatibility through compile options"""
        with open(compile_commands_path, encoding="utf-8") as f:
            compile_commands = json.load(f)
            for compilation_unit in compile_commands:
                assert "-mmacosx-version-min=10.9" in compilation_unit["command"]

    @unittest.skipUnless(IS_MACOS, "Only relevant for macOS")
    def test_macos_universal_binaries(self):
        """macOS supports Universal Binaries that works for both Intel and Mx"""
        with open(compile_commands_path, encoding="utf-8") as f:
            compile_commands = json.load(f)
            for compilation_unit in compile_commands:
                assert "-arch x86_64" in compilation_unit["command"]
                assert "-arch arm64" in compilation_unit["command"]


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--compile_commands", required=True)
    # This is a hack to make custom parameters work
    # See https://stackoverflow.com/a/8660290
    parser.add_argument("unittest_args", nargs="*")
    args = parser.parse_args()
    compile_commands_path = args.compile_commands
    sys.argv[1:] = args.unittest_args
    unittest.main(argv=sys.argv, verbosity=2)
