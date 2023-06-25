# Enable .env integration
set dotenv-load

# Choose conan profile based on platform
conan_profile := if os() == "macos" {
    "macos"
} else if os() == "windows" {
    "windows"
} else if os() == "linux" {
    if arch() == "aarch64" {
        "linux-arm"
    } else {
        "linux"
    }
} else {
    "default"
}

# Cleanup build, temp and all generated files
[unix]
cleanup:
    rm -rf build
    rm -f CMakeUserPresets.json
    rm -rf test/build
    rm -f test/CMakeUserPresets.json
    rm -rf Testing
    rm -rf ~/.conan2/p
    rm -f config/system/requirements.macos.brew.rb.lock.json

# Cleanup build, temp and all generated files
[windows]
cleanup:
    echo "Cleaning is not supported on Windows yet"
    
[macos]
[private]
setup-system:
    brew update
    brew bundle install --file=config/system/requirements.macos.brew.rb --no-lock

[windows]
[private]
setup-system:
    choco install config/system/requirements.windows.choco.config --ignore-package-exit-codes

[linux]
[private]
setup-system:
    sudo apt-get update # Update available packages list
    xargs -a config/system/requirements.linux.apt.txt sudo apt-get install -y

[private]
setup-git:
    git submodule update --init --recursive

[private]
setup-conan:
    pip3 install -r config/system/requirements.dev.pip.txt
    conan profile detect --force
    conan config install config/conan
    conan create modules/juce-conan -pr:h {{conan_profile}} -pr:b {{conan_profile}}
    conan create modules/pluginval-conan -pr:h {{conan_profile}} -pr:b {{conan_profile}}

# Auth in macOS notarytool. Needed to notarize dmg
[private]
[macos]
setup-notarytool:
    xcrun notarytool store-credentials "APPLE_SIGN_PROFILE" --apple-id $MACOS_APPLE_ID --password $MACOS_APPLE_PASSWORD --team-id $MACOS_APPLE_TEAM_ID

# Setup the project
setup: cleanup
    just setup-system # Install system dependencies
    just setup-git    # Initialize git dependencies
    just setup-conan  # Setup conan & install project dependencies

# Build, sign and bundle the project
build:
    conan install . -pr:h {{conan_profile}} -pr:b {{conan_profile}}
    conan build . -pr:h {{conan_profile}} -pr:b {{conan_profile}}

# Run Static Code Analysis
sca:
    cppcheck --project=build/Release/compile_commands.json --enable=all --report-progress --check-config --suppress=missingIncludeSystem

# Runs plugin as a standalone app
[macos]
run:
    open build/Release/peakeater_artefacts/Release/Standalone/peakeater.app

# Run a test suite
# macOS requires a special tratment because how Mac works with AU.
# macOS requires AU plugins to be registered in order to be discoverable.
# Registration means it is placed either in system or in user's AU folder and
# special utility is called.
# Discussion - https://github.com/Tracktion/pluginval/issues/39
[macos]
test:
    # Cleanup leftovers from previous test
    rm -rf /Library/Audio/Plug-Ins/Components/peakeater.component
    # Copy AU to the user AU plugins folder(/Library/Audio/Plug-Ins/Components) because macOS makes a scan there
    # Copy to the the global AU dir because user-dir is not supported on a CI
    cp -R build/Release/peakeater_artefacts/Release/AU/peakeater.component /Library/Audio/Plug-Ins/Components
    # Trigger AudioComponentRegistrar and auval, this will force macOS to scan & register new AU plugin
    killall -9 AudioComponentRegistrar
    auval -a
    # Finally, we can test. Source conanbuild.sh because path to pluginval is set there
    source build/Release/generators/conanbuild.sh && ctest --progress --verbose --test-dir build/Release
    # Cleanup at the end
    rm -rf /Library/Audio/Plug-Ins/Components/peakeater.component

# Run a test suite
[linux]
test:
    # Source conanbuild.sh because path to pluginval is set there
    . build/Release/generators/conanbuild.sh && ctest --progress --verbose --test-dir build/Release

# Run a test suite
[windows]
test:
    # Run conanbuild.bat because path to pluginval is set there
    .\build\Release\generators\conanbuild.bat && ctest --progress --verbose --test-dir build/Release
