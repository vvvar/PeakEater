"""
Create executable(for Windows) that installs VST3.
@note Python v3.8+ is required for this to run.
"""
import argparse
import utils

# WiX Default Configuration
WIX_COMPILER_DEFAULT_PATH = "C:\\Program Files (x86)\\WiX Toolset v3.11\\bin\\candle.exe"
WIX_LINKER_DEFAULT_PATH = "C:\\Program Files (x86)\\WiX Toolset v3.11\\bin\\light.exe"

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
utils.log_verbose("Arguments: ", args)

# Create tmp dir
utils.log_info("Ensuring tmp and release dirs are clean and esists...")

TMP_DIR_PATH = utils.get_tmp_dir_path()
utils.ensure_dir_empty(TMP_DIR_PATH)
RELEASE_DIR_PATH = utils.get_release_dir_path()
utils.ensure_dir_empty(RELEASE_DIR_PATH)

# Copy VST3, AU, configs and assets into it
utils.log_info("Copying VST3, release assets and configs to tmp dir...")
utils.copy_dir_content_recursive(
    utils.get_build_vst3_dir_path(args.release_type), TMP_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_project_release_assets_dir_path(), TMP_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_project_release_configs_dir_path(), TMP_DIR_PATH)

# Create installer
utils.log_info("Creating installer...")
wixCompilerFullPath = utils.create_path(args.wix_compiler_path)
wixLinkerFullPath = utils.create_path(args.wix_linker_path)
wixConfigFullPath = TMP_DIR_PATH.joinpath("wix-config.wxs").resolve()
wixObjectFullPath = TMP_DIR_PATH.joinpath("wix-config.wixobj").resolve()
msiBuildFullPath = RELEASE_DIR_PATH.joinpath("PeakEater.msi").resolve()
utils.exec_command(
    f'""{str(wixCompilerFullPath)}" "{str(wixConfigFullPath)}" -o "{str(wixObjectFullPath)}" -arch "x64""')
utils.exec_command(
    f'""{str(wixLinkerFullPath)}" "{str(wixObjectFullPath)}" -o "{str(msiBuildFullPath)}" -ext "WixUIExtension""')

# Conditionally, cleanup tmp
if not args.preserve_tmp:
    utils.log_info("Cleaning up tmp dir...")
    utils.rm_dir(TMP_DIR_PATH)
