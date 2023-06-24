# Enable .env integration
set dotenv-load

# Choose conan profile based on platform
conan_profile := if os() == "macos" {
    "macos"
} else if os() == "windows" {
    "windows"
} else if os() == "linux" {
    "linux"
} else {
    "default"
}

# When MACOS_APPLE_DEVELOPER_ID is set then we can codesign
codesign := if env_var_or_default("MACOS_APPLE_DEVELOPER_ID", "") == "" {
    "False"
} else {
    "True"
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
    conan install . -pr:h {{conan_profile}} -pr:b {{conan_profile}} -o signed={{codesign}}
    conan build . -pr:h {{conan_profile}} -pr:b {{conan_profile}} -o signed={{codesign}}

# Run Static Code Analysis
sca:
    cppcheck --project=build/Release/compile_commands.json --enable=all --report-progress --check-config --suppress=missingIncludeSystem

# Runs plugin as a standalone app
[macos]
run:
    open build/Release/peakeater_artefacts/Release/Standalone/peakeater.app

# Package an application as a Conan package and test it with test project
test:
    conan export-pkg . -pr:h {{conan_profile}} -pr:b {{conan_profile}} -tf test -c tools.cmake.cmake_layout:build_folder_vars="['settings.build_type']"
