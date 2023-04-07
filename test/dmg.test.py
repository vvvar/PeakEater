import unittest
import subprocess
import os
import sys


def isMac():
    return sys.platform == "darwin"


class DmgTestSuite(unittest.TestCase):
    """Make sure DMG file is correct"""

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
    unittest.main()
