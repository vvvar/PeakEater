from os import system, mkdir
from pathlib import Path
from enum import Enum
from shutil import rmtree, copytree

VERBOSE_LOGGING = False


def logInfo(*args, **kwargs):
    print(*args, **kwargs)


def logVerbose(*args, **kwargs):
    if VERBOSE_LOGGING:
        print(*args, **kwargs)

# Represents relese type later used for arguments parsing


class ReleaseType(Enum):
    debug = 'Debug'
    release = 'Release'

    def __str__(self):
        return self.value

    def __add__(self, other):
        return self.value + other

    def __radd__(self, other):
        return other + self.value

# Executes provided shell command


def execCommand(command: str) -> None:
    logVerbose("Executing command: " + command)
    outout = system(command)
    if (outout):
        logInfo(outout)


def getProjectRootDirPath() -> Path:
    path = Path(__file__).parent.parent.parent.resolve()
    logVerbose("Project root dir: " + str(path))
    return path


def getProjectReleaseAssetsDirPath() -> Path:
    path = getProjectRootDirPath().joinpath("Scripts/Release/assets").resolve()
    logVerbose("Project release assets dir path: " + str(path))
    return path


def getProjectReleaseConfigsDirPath() -> Path:
    path = getProjectRootDirPath().joinpath("Scripts/Release/configs").resolve()
    logVerbose("Project release configs dir path: " + str(path))
    return path

# Returns path to project's build dir


def getBuildDirPath() -> Path:
    path = getProjectRootDirPath().joinpath("build").resolve()
    logVerbose("Project build dir: " + str(path))
    return path

# Returns path AU bin that was build


def getBuildAUDirPath(releaseType: ReleaseType) -> Path:
    path = getBuildDirPath().joinpath(
        "PeakEater_artefacts/" + releaseType + "/AU").resolve()
    logVerbose("AU build dir: " + str(path))
    return path

# Returns path VST3 bin that was build


def getBuildVST3DirPath(releaseType: ReleaseType) -> Path:
    path = getBuildDirPath().joinpath("PeakEater_artefacts/" +
                                      releaseType + "/VST3").resolve()
    logVerbose("VST3 build dir: " + str(path))
    return path

# Get path to temp dir


def getTmpDirPath() -> Path:
    path = getBuildDirPath().joinpath("__tmp__").resolve()
    logVerbose("tmp build dir: " + str(path))
    return path

# Get path to release artifacts dir


def getReleaseDirPath() -> Path:
    path = getBuildDirPath().joinpath("release").resolve()
    logVerbose("Release articats dir: " + str(path))
    return path

# Ensures that dor exists and empty


def ensureDirEmpty(path: Path) -> None:
    if (path.exists()):
        rmtree(path)
    mkdir(path)


def rmDir(path: Path) -> None:
    if (path.exists()):
        rmtree(path)


def copyDirContentRecursive(src: Path, dst: Path) -> None:
    copytree(src=src, dst=dst, dirs_exist_ok=True)
