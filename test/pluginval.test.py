import unittest
import subprocess
import os
import sys
import ssl
import shutil
import tempfile
from io import BytesIO
from urllib.request import urlopen
from zipfile import ZipFile
import logging


def is_mac():
    return sys.platform == "darwin"


def is_win():
    return sys.platform == "win32"


def is_linux():
    return sys.platform == "linux" or sys.platform == "linux2"


class TestPluginval(unittest.TestCase):
    """Make sure plugins passes pluginval"""

    """URL: https://github.com/Tracktion/pluginval"""

    logger = logging.getLogger(__name__)
    logging.basicConfig(format="%(asctime)s %(module)s %(levelname)s: %(message)s", datefmt="%m/%d/%Y %I:%M:%S %p", level=logging.INFO)

    @property
    def download_url(self):
        if is_mac():
            return "https://github.com/Tracktion/pluginval/releases/download/v1.0.3/pluginval_macOS.zip"
        elif is_win():
            return "https://github.com/Tracktion/pluginval/releases/download/v1.0.3/pluginval_Windows.zip"
        elif is_linux():
            return "https://github.com/Tracktion/pluginval/releases/download/v1.0.3/pluginval_Linux.zip"
        else:
            raise RuntimeError("Unknown platform")

    @property
    def pluginval_bin(self):
        if is_mac():
            return f"{self.pluginval_dir}/pluginval.app/Contents/MacOS/pluginval"
        elif is_win():
            return f"{self.pluginval_dir}/pluginval.exe"
        elif is_linux():
            return f"{self.pluginval_dir}/pluginval"
        else:
            raise RuntimeError("Unknown platform")

    def setUp(self):
        """Download pluginval for host platform"""
        ctx = ssl.create_default_context()
        ctx.check_hostname = False
        ctx.verify_mode = ssl.CERT_NONE
        self.logger.info(f"Downloading pluginval from {self.download_url}")
        with urlopen(self.download_url, context=ctx) as zipresp:
            with ZipFile(BytesIO(zipresp.read())) as zfile:
                self.pluginval_dir = f"{tempfile.gettempdir()}/pluginval"
                if os.path.exists(self.pluginval_dir):
                    shutil.rmtree(self.pluginval_dir)
                self.logger.info(f"Extracting pluginval into {self.pluginval_dir}...")
                zfile.extractall(self.pluginval_dir)
                os.chmod(self.pluginval_bin, 0o777)

    def test_vst3(self):
        """Make sure VST3 passed pluginval"""
        vst3_path = os.path.abspath(os.path.join(__file__, "../..", "build", "Release", "peakeater_artefacts", "Release", "VST3", "peakeater.vst3"))
        self.assertTrue(os.path.exists(vst3_path))
        process = subprocess.Popen([self.pluginval_bin, "--strictness-level", "10", "--verbose", "--skip-gui-tests", vst3_path])
        process.wait()
        self.assertEqual(0, process.returncode)

    @unittest.skipUnless(is_mac(), "AU is only supported by macOS")
    def test_au(self):
        """Make sure AU passed pluginval"""
        au_path = os.path.abspath(os.path.join(__file__, "../..", "build", "Release", "peakeater_artefacts", "Release", "AU", "peakeater.component"))
        self.assertTrue(os.path.exists(au_path))
        process = subprocess.Popen([self.pluginval_bin, "--strictness-level", "10", "--verbose", "--skip-gui-tests", au_path])
        process.wait()
        self.assertEqual(0, process.returncode)


if __name__ == "__main__":
    unittest.main(verbosity=2)
