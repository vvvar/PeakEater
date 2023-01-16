"""
Create DMG Image(for MacOS) that contains release artifacts.
@note Python v3.8+ is required for this to run.
"""
import argparse
import utils
from os import environ
from dotenv import load_dotenv

# Read env variables
load_dotenv()

# Read input arguments
argument_parser = argparse.ArgumentParser()
argument_parser.add_argument('--release_type',
                             type=utils.ReleaseType, default=utils.ReleaseType.release,
                             choices=list(utils.ReleaseType),
                             help='Type of release package, e.g. Release or Debug')
argument_parser.add_argument('--sign_and_notarize',
                             type=bool,
                             default=False,
                             help='Sign all binaries and notarize disk image')
argument_parser.add_argument('--preserve_tmp',
                             type=bool,
                             default=False,
                             help='Preserve temp dir after script executed')
args = argument_parser.parse_args()
# Print arguments
utils.log_verbose("Arguments: ", args)

# Ensure appdmg installed
utils.log_info("Installing appdmg...")
utils.exec_command("npm install -g appdmg")

# Create tmp dir
utils.log_info("Ensuring tmp and release dirs are clean and esists...")
TMP_DIR_PATH = utils.get_tmp_dir_path()
utils.ensure_dir_empty(TMP_DIR_PATH)
RELEASE_DIR_PATH = utils.get_release_dir_path()
utils.ensure_dir_empty(RELEASE_DIR_PATH)

# Copy VST3, AU, configs and assets into it
utils.log_info("Copying VST3, AU, release assets and configs to tmo dir...")
utils.copy_dir_content_recursive(
    utils.get_build_vst3_dir_path(args.release_type), TMP_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_build_au_dir_path(args.release_type), TMP_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_build_lv2_dir_path(args.release_type), TMP_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_build_clap_dir_path(args.release_type), TMP_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_project_release_assets_dir_path(), TMP_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_project_release_configs_dir_path(), TMP_DIR_PATH)

if args.sign_and_notarize:
    # Code-sign binaries
    utils.exec_command(
        f"codesign --force -s '{environ.get('MACOS_APPLE_IDENTITY')}' -v {str(TMP_DIR_PATH) + '/PeakEater.vst3'} --deep --strict --options=runtime --timestamp")
    utils.exec_command(
        f"codesign --force -s '{environ.get('MACOS_APPLE_IDENTITY')}' -v {str(TMP_DIR_PATH) + '/PeakEater.component'} --deep --strict --options=runtime --timestamp")
    utils.exec_command(
        f"codesign --force -s '{environ.get('MACOS_APPLE_IDENTITY')}' -v {str(TMP_DIR_PATH) + '/PeakEater.lv2/libPeakEater.so'} --deep --strict --options=runtime --timestamp")
    utils.exec_command(
        f"codesign --force -s '{environ.get('MACOS_APPLE_IDENTITY')}' -v {str(TMP_DIR_PATH) + '/PeakEater.clap'} --deep --strict --options=runtime --timestamp")

# Create .dmg
utils.log_info("Creating release image...")
APP_DMG_CONFIG_FILE_PATH = str(TMP_DIR_PATH) + "/appdmg-config.json"
OUTPUT_DMG_FILE_PATH = str(RELEASE_DIR_PATH) + "/PeakEater.dmg"
utils.exec_command("appdmg " + APP_DMG_CONFIG_FILE_PATH +
                   " " + OUTPUT_DMG_FILE_PATH)

if args.sign_and_notarize:
    # Code-sign .dmg
    utils.exec_command(
        f"codesign --force -s '{environ.get('MACOS_APPLE_IDENTITY')}' -v {OUTPUT_DMG_FILE_PATH} --deep --strict --timestamp")
    # Notarize .dmg
    utils.exec_command(
        f"xcrun notarytool submit {OUTPUT_DMG_FILE_PATH} --apple-id {environ.get('MACOS_APPLE_ID')} --password {environ.get('MACOS_APPLE_PASSWORD')} --team-id {environ.get('MACOS_APPLE_TEAM_ID')} --wait")

# Staple .dmg
utils.exec_command(f"xcrun stapler staple {OUTPUT_DMG_FILE_PATH}")

# Conditionally, cleanup tmp
if not args.preserve_tmp:
    utils.log_info("Cleaning up tmp dir...")
    utils.rm_dir(TMP_DIR_PATH)

# Done, print where DMG is located
utils.log_info("Done! Dmg file may be found at: " + OUTPUT_DMG_FILE_PATH)
