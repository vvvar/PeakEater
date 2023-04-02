from os import system, mkdir, name
from pathlib import Path
from enum import Enum
from shutil import rmtree, copytree

VERBOSE_LOGGING = False


def log_info(*args, **kwargs):
    """Log to info channel"""
    print(*args, **kwargs)


def log_verbose(*args, **kwargs):
    """Log to verbose channel"""
    if VERBOSE_LOGGING:
        print(*args, **kwargs)


def is_windows():
    """Check is OS Windows"""
    return name == 'nt'


class ReleaseType(Enum):
    """Represents relese type later used for arguments parsing"""
    debug = 'Debug'
    release = 'Release'

    def __str__(self):
        return self.value

    def __add__(self, other):
        return self.value + other

    def __radd__(self, other):
        return other + self.value


def exec_command(command):
    """Executes provided shell command"""
    log_verbose("Executing command: " + command)
    log_info(system(command))


def get_project_root_dir_path():
    """Get path to project root"""
    path = Path(__file__).parent.parent.parent.resolve()
    log_verbose("Project root dir: " + str(path))
    return path


def get_project_release_assets_dir_path():
    """Get path to project assets dir"""
    path = get_project_root_dir_path().joinpath("Scripts/Release/assets").resolve()
    log_verbose("Project release assets dir path: " + str(path))
    return path


def get_project_release_configs_dir_path():
    """Get path to project config dir"""
    path = get_project_root_dir_path().joinpath("Scripts/Release/configs").resolve()
    log_verbose("Project release configs dir path: " + str(path))
    return path


def get_build_dir_path():
    """Returns path to project's build dir"""
    path = get_project_root_dir_path().joinpath("build").resolve()
    log_verbose("Project build dir: " + str(path))
    return path


def get_build_au_dir_path(release_type):
    """Returns path AU bin that was build"""
    path = get_build_dir_path().joinpath(
        "PeakEater_artefacts/" + release_type + "/AU").resolve()
    log_verbose("AU build dir: " + str(path))
    return path


def get_build_vst3_dir_path(release_type):
    """Returns path VST3 bin that was build"""
    path = get_build_dir_path().joinpath("PeakEater_artefacts/" +
                                         release_type + "/VST3").resolve()
    log_verbose("VST3 build dir: " + str(path))
    return path


def get_build_lv2_dir_path(release_type):
    """Returns path LV2 bin that was build"""
    path = get_build_dir_path().joinpath("PeakEater_artefacts/" +
                                         release_type + "/LV2").resolve()
    log_verbose("LV2 build dir: " + str(path))
    return path


def get_build_clap_dir_path(release_type):
    """Returns path CLAP bin that was build"""
    path = get_build_dir_path().joinpath("PeakEater_artefacts/" +
                                         release_type + "/CLAP").resolve()
    log_verbose("CLAP build dir: " + str(path))
    return path


def get_tmp_dir_path():
    """Get path to temp dir"""
    path = get_build_dir_path().joinpath("__tmp__").resolve()
    log_verbose("tmp build dir: " + str(path))
    return path


def get_release_dir_path():
    """Get path to release artifacts dir"""
    path = get_build_dir_path().joinpath("release").resolve()
    log_verbose("Release articats dir: " + str(path))
    return path


def ensure_dir_empty(path):
    """Ensures that dor exists and empty"""
    if path.exists():
        rmtree(path)
    mkdir(path)


def rm_dir(dir_path):
    """Remove dir by specified path"""
    if dir_path.exists():
        rmtree(dir_path)


def copy_dir_content_recursive(src, dst):
    """Copies content of dir with all subcontent"""
    copytree(src=src, dst=dst, dirs_exist_ok=True)


def create_path(src):
    """Construct Path from str"""
    return Path(src).resolve()


def sanitize_path(path):
    """Removes all special characters from path"""
    return Path(str(path).replace(" ", "\\")).resolve()
