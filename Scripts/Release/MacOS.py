"""
Create DMG Image(for MacOS) that contains release artifacts.
@note Python v3.8+ is required for this to run.
"""
from ctypes import util
import utils
import argparse
# Parse input arguments
argument_parser = argparse.ArgumentParser()
argument_parser.add_argument('--force_upgrade_dependencies', type=bool,
                             default=False, help='Force update global env dependencies live xcode, brew, node, etc')
argument_parser.add_argument('--release_type', type=utils.ReleaseType, default=utils.ReleaseType.release,
                             choices=list(utils.ReleaseType), help='Type of release package, e.g. Release or Debug')
argument_parser.add_argument('--preserve_tmp', type=bool, default=False,
                             help='Preserve temp dir after script executed')
args = argument_parser.parse_args()
# Print arguments
utils.logVerbose("Arguments: ", args)
# Conditionally, force install env dependencies
if (args.force_upgrade_dependencies):
    utils.logInfo("Force upgrading dependencies...")
    utils.execCommand("softwareupdate --all --install --force")
    utils.execCommand("brew update")
    utils.execCommand("brew upgrade")
    utils.execCommand("brew install node")
# Ensure appdmg installed
utils.logInfo("Installing appdmg...")
utils.execCommand("npm install -g appdmg")
# Create tmp dir
utils.logInfo("Ensuring tmp and release dirs are clean and esists...")
TMP_DIR_PATH = utils.getTmpDirPath()
utils.ensureDirEmpty(TMP_DIR_PATH)
RELEASE_DIR_PATH = utils.getReleaseDirPath()
utils.ensureDirEmpty(RELEASE_DIR_PATH)
# Copy VST3, AU, configs and assets into it
utils.logInfo("Copying VST3, AU, release assets and configs to tmo dir...")
utils.copyDirContentRecursive(
    utils.getBuildVST3DirPath(args.release_type), TMP_DIR_PATH)
utils.copyDirContentRecursive(
    utils.getBuildAUDirPath(args.release_type), TMP_DIR_PATH)
utils.copyDirContentRecursive(
    utils.getProjectReleaseAssetsDirPath(), TMP_DIR_PATH)
utils.copyDirContentRecursive(
    utils.getProjectReleaseConfigsDirPath(), TMP_DIR_PATH)
# Create DMG
utils.logInfo("Creating release image...")
appDmgConfigFilePath = str(TMP_DIR_PATH) + "/appdmg-config.json"
outputDmgFilePath = str(RELEASE_DIR_PATH) + "/PeakEater.dmg"
utils.execCommand("appdmg " + appDmgConfigFilePath + " " + outputDmgFilePath)
# Conditionally, cleanup tmp
if not args.preserve_tmp:
    utils.logInfo("Cleaning up tmp dir...")
    utils.rmDir(TMP_DIR_PATH)
# Done, print where DMG is located
utils.logInfo("Done! Dmg file may be found at: " + outputDmgFilePath)
