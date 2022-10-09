"""
Create executable(for Windows) that installs VST3.
@note Python v3.8+ is required for this to run.
"""
from ctypes import util
import utils
import argparse
from pathlib import Path
# WiX Default Configuration
WIX_COMPILER_DEFAULT_PATH = "C:\Program Files (x86)\\WiX Toolset v3.11\\bin\\candle.exe"
WIX_LINKER_DEFAULT_PATH = "C:\Program Files (x86)\\WiX Toolset v3.11\\bin\\light.exe"
# Parse input arguments
argument_parser = argparse.ArgumentParser()
argument_parser.add_argument(
    '--wix_compiler_path', type=str, default=WIX_COMPILER_DEFAULT_PATH, help='Path to WiX Compiler(usually called candle.exe)')
argument_parser.add_argument(
    '--wix_linker_path', type=str, default=WIX_LINKER_DEFAULT_PATH, help='Path to WiX Linker(usually called light.exe)')
argument_parser.add_argument('--release_type', type=utils.ReleaseType, default=utils.ReleaseType.release,
                             choices=list(utils.ReleaseType), help='Type of release package, e.g. Release or Debug')
argument_parser.add_argument('--preserve_tmp', type=bool, default=False,
                             help='Preserve temp dir after script executed')
args = argument_parser.parse_args()
# Print arguments
utils.logVerbose("Arguments: ", args)
# Create tmp dir
utils.logInfo("Ensuring tmp and release dirs are clean and esists...")
TMP_DIR_PATH = utils.getTmpDirPath()
utils.ensureDirEmpty(TMP_DIR_PATH)
RELEASE_DIR_PATH = utils.getReleaseDirPath()
utils.ensureDirEmpty(RELEASE_DIR_PATH)
# Copy VST3, AU, configs and assets into it
utils.logInfo("Copying VST3, release assets and configs to tmp dir...")
utils.copyDirContentRecursive(
    utils.getBuildVST3DirPath(args.release_type), TMP_DIR_PATH)
utils.copyDirContentRecursive(
    utils.getProjectReleaseAssetsDirPath(), TMP_DIR_PATH)
utils.copyDirContentRecursive(
    utils.getProjectReleaseConfigsDirPath(), TMP_DIR_PATH)
# Create installer
utils.logInfo("Creating installer...")
wixCompilerFullPath = utils.createPath(args.wix_compiler_path)
wixLinkerFullPath = utils.createPath(args.wix_linker_path)
wixConfigFullPath = TMP_DIR_PATH.joinpath("wix-config.wxs").resolve()
wixObjectFullPath = TMP_DIR_PATH.joinpath("wix-config.wixobj").resolve()
msiBuildFullPath = RELEASE_DIR_PATH.joinpath("PeakEater.msi").resolve()
utils.execCommand(
    f'""{str(wixCompilerFullPath)}" "{str(wixConfigFullPath)}" -o "{str(wixObjectFullPath)}" -arch "x64""')
utils.execCommand(
    f'""{str(wixLinkerFullPath)}" "{str(wixObjectFullPath)}" -o "{str(msiBuildFullPath)}" -ext "WixUIExtension""')
# Conditionally, cleanup tmp
if not args.preserve_tmp:
    utils.logInfo("Cleaning up tmp dir...")
    utils.rmDir(TMP_DIR_PATH)
