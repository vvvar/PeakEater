"""
Create DMG Image(for MacOS) that contains release artifacts.
@note Python v3.8+ is required for this to run.
"""
import argparse
import utils
import os
import pathlib
import shutil
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
argument_parser.add_argument('--release_version',
                             type=str,
                             default=False,
                             help='Release version to put into installer')
args = argument_parser.parse_args()
# Print arguments
utils.log_verbose("Arguments: ", args)

# Create tmp dir
utils.log_info("Ensuring tmp and release dirs are clean and esists...")
RELEASE_DIR_PATH = utils.get_release_dir_path()
utils.ensure_dir_empty(RELEASE_DIR_PATH)

# Copy VST3, AU, configs and assets into it
utils.log_info("Copying plugins to the release dir...")
utils.copy_dir_content_recursive(
    utils.get_build_vst3_dir_path(args.release_type), RELEASE_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_build_au_dir_path(args.release_type), RELEASE_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_build_lv2_dir_path(args.release_type), RELEASE_DIR_PATH)
utils.copy_dir_content_recursive(
    utils.get_build_clap_dir_path(args.release_type), RELEASE_DIR_PATH)


def codesign(bin_path, resursive = False):
    """Codesigns bin"""
    utils.log_info(f"Codesigning {bin_path}...")
    identity = os.environ.get('MACOS_APPLE_IDENTITY')
    if resursive:
        for filename in os.listdir(bin_path):
            utils.exec_command(
                f"codesign --force -s '{identity}' -v {os.path.join(bin_path, filename)} --deep --strict --options=runtime --timestamp")
    else:
        utils.exec_command(
            f"codesign --force -s '{identity}' -v {bin_path} --deep --strict --options=runtime --timestamp")


def notarize(bin_path):
    """Notarize and stable bin"""
    apple_id = os.environ.get('MACOS_APPLE_ID')
    password = os.environ.get('MACOS_APPLE_PASSWORD')
    team_id = os.environ.get('MACOS_APPLE_TEAM_ID')
    utils.exec_command(
        f"xcrun notarytool submit {bin_path} --apple-id {apple_id} --password {password} --team-id {team_id} --wait")
    utils.exec_command(f"xcrun stapler staple {bin_path}")


if args.sign_and_notarize:
    codesign(f"{str(RELEASE_DIR_PATH)}/PeakEater.vst3")
    codesign(f"{str(RELEASE_DIR_PATH)}/PeakEater.component")
    codesign(f"{str(RELEASE_DIR_PATH)}/PeakEater.lv2", resursive=True)
    codesign(f"{str(RELEASE_DIR_PATH)}/PeakEater.clap")

utils.log_info("Creating DMG image...")
release_dmg_path = f"{str(RELEASE_DIR_PATH)}/PeakEater-{args.release_version}.dmg"
utils.exec_command("npm install -g appdmg")
shutil.copyfile(
    f"{os.path.dirname(__file__)}/configs/appdmg-config.json", f"{RELEASE_DIR_PATH}/appdmg-config.json")
utils.exec_command(
    f"appdmg {RELEASE_DIR_PATH}/appdmg-config.json {str(RELEASE_DIR_PATH)}/PeakEater-{args.release_version}.dmg")
if args.sign_and_notarize:
    codesign(release_dmg_path)
    notarize(release_dmg_path)

utils.rm_dir(pathlib.Path(f"{str(RELEASE_DIR_PATH)}/PeakEater.vst3"))
utils.rm_dir(pathlib.Path(f"{str(RELEASE_DIR_PATH)}/PeakEater.component"))
utils.rm_dir(pathlib.Path(f"{str(RELEASE_DIR_PATH)}/PeakEater.lv2"))
utils.rm_dir(pathlib.Path(f"{str(RELEASE_DIR_PATH)}/PeakEater.clap"))

utils.log_info("Done! Dmg file may be found at: " + release_dmg_path)
