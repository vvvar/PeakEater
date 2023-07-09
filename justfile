# Enable .env integration
set dotenv-load

# Choose conan profile based on platform
conan_profile := if os() == "macos" {
    "config/conan/macos.jinja"
} else if os() == "windows" {
    "config/conan/windows.jinja"
} else if os() == "linux" {
    "config/conan/linux.jinja"
} else {
    "default"
}

build_type := if env_var_or_default("DEBUG", "False") == "False" { "Release" } else { "Debug" }

# Just uses "sh" on Windows by default.
# Force use powershell since Conan fails to lift virtualenv with sh
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

# Cleanup tmp and gen files in the repository
[unix]
[private]
clean-repo:
    rm -rf build
    rm -f CMakeUserPresets.json
    rm -rf test/build
    rm -f test/CMakeUserPresets.json
    rm -rf Testing
    rm -f config/system/requirements.macos.brew.rb.lock.json

# Cleanup tmp and gen files in the repository
[windows]
[private]
clean-repo:
    echo "Cleaning is not supported on Windows yet"
    
# Clean Conan cache(e.g. ~/conan/data)
[unix]
[private]
clean-conan-cache:
    rm -rf ~/conan/data
    rm -rf ~/conan2/data

[private]
clean-all:
    just clean-repo
    just clean-conan-cache

[macos]
[private]
setup-system:
    brew update
    brew bundle install --file=config/system/requirements.macos.brew.rb --no-lock

[windows]
[private]
setup-system:
    # Without it PowerShell will ask any .ps1 script to be digitally signed
    Set-ExecutionPolicy -ExecutionPolicy unrestricted
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
    conan create modules/juce-conan -pr:h {{conan_profile}} -pr:b {{conan_profile}}
    conan create modules/pluginval-conan -pr:h {{conan_profile}} -pr:b {{conan_profile}}

# Auth in macOS notarytool. Needed to notarize dmg
[macos]
[private]
setup-notarytool:
    xcrun notarytool store-credentials "APPLE_SIGN_PROFILE" --apple-id $MACOS_APPLE_ID --password $MACOS_APPLE_PASSWORD --team-id $MACOS_APPLE_TEAM_ID

# Setup the project
setup:
    just setup-system # Install system dependencies
    just setup-git    # Initialize git dependencies
    just setup-conan  # Setup conan & install project dependencies

# Build, sign and bundle the project
build:
    conan install . -pr:h {{conan_profile}} -pr:b {{conan_profile}}
    conan build . -pr:h {{conan_profile}} -pr:b {{conan_profile}}

# Run Static Code Analysis
sca:
    cppcheck --project=build/{{build_type}}/compile_commands.json --enable=all --report-progress --check-config --suppress=missingIncludeSystem

# Runs plugin as a standalone app
[macos]
run:
    build/{{build_type}}/peakeater_artefacts/{{build_type}}/Standalone/peakeater.app/Contents/MacOS/peakeater

# Runs plugin as a standalone app
[windows]
run:
    build/{{build_type}}/peakeater_artefacts/{{build_type}}/Standalone/peakeater.exe

# macOS requires a special treatment because how Mac works with AU.
# macOS requires AU plugins to be registered in order to be discoverable.
# Registration means it is placed either in system or in user's AU folder and
# special utility is called.
# Discussion - https://github.com/Tracktion/pluginval/issues/39
[macos]
[private]
test-au:
    # In case there's no AU user dir
    mkdir -p ~/Library/Audio/Plug-Ins/Components
    # Cleanup leftovers from previous test
    rm -rf ~/Library/Audio/Plug-Ins/Components/peakeater.component
    # Copy AU to the user AU plugins folder(~/Library/Audio/Plug-Ins/Components) because macOS makes a scan there
    cp -R build/{{build_type}}/peakeater_artefacts/{{build_type}}/AU/peakeater.component ~/Library/Audio/Plug-Ins/Components
    # Trigger AudioComponentRegistrar and auval, this will force macOS to scan & register new AU plugin
    killall -9 AudioComponentRegistrar
    auval -a
    # Source conanbuild.sh because path to pluginval is set there
    source build/{{build_type}}/generators/conanbuild.sh && ctest --progress --verbose --test-dir build/{{build_type}} -R Pluginval_AU
    # Cleanup global Components dir since we've polluted it with our AU under test
    rm -rf ~/Library/Audio/Plug-Ins/Components/peakeater.component

[macos]
[private]
test-vst3:
    source build/{{build_type}}/generators/conanbuild.sh && ctest --progress --verbose --test-dir build/{{build_type}} -R Pluginval_VST3

[macos]
[private]
test-compile-options:
    source build/{{build_type}}/generators/conanbuild.sh && ctest --progress --verbose --test-dir build/{{build_type}} -R CompileOptions

# Run a test suite
# On a mac, we want to have an ability to exec every test individually because AU test is not possible with sanitized exec
[macos]
test:
    just test-compile-options
    just test-vst3
    just test-au
    
# Run a test suite
[linux]
test:
    # Source conanbuild.sh because path to pluginval is set there
    . build/{{build_type}}/generators/conanbuild.sh && ctest --progress --verbose --test-dir build/{{build_type}}

# Run a test suite
[windows]
test:
    # Run conanbuild.ps1 because path to pluginval is set there
    ./build/{{build_type}}/generators/conanbuild.ps1; ctest --progress --verbose --test-dir build/{{build_type}}
