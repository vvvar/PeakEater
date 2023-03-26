@echo off

@rem Validate arguments 
set CMAKE_BUILD_TYPE=%1
if not defined CMAKE_BUILD_TYPE (
    echo Build type is not provided. Please pass it as first argument. E.g. Release, Debug
    exit /b 1
)
set CMAKE_BUILD_TARGET=%1
if not defined CMAKE_BUILD_TARGET (
    echo Build target is not provided. Please pass it as second argument. E.g. PeakEater_VST3, PeakEater_AU, PeakEater_LV2, PeakEater_CLAP, all
    exit /b 1
)

echo ========== Windows ==========
echo Build type: %CMAKE_BUILD_TYPE%
echo Build target: %CMAKE_BUILD_TARGET%
echo =============================

@rem Print all commands
echo on

@rem Configure
set PROJECT_ROOT=%~dp0\..\..
call cmake %PROJECT_ROOT% ^
        -B %PROJECT_ROOT%\build ^
        -G "Visual Studio 17 2022" -A x64 ^
        -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE ^
        -DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE% \
        -DJUCE_BUILD_EXAMPLES=OFF -DJUCE_BUILD_EXTRAS=ON ^
        || exit /b 1

@rem Build
call cmake --build %PROJECT_ROOT%\build --config %CMAKE_BUILD_TYPE% --target %CMAKE_BUILD_TARGET% || exit /b 1