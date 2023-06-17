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
    conan config install config/global.conf
    conan create modules/juce-conan
    conan create modules/pluginval-conan
    conan install .

# Setup the project
setup: cleanup
    just setup-system # Install system dependencies
    just setup-git    # Initialize git dependencies
    just setup-conan  # Setup conan & install project dependencies

# Build, sign and bundle the project
[macos]
build:
    conan build . -o signed=True

# Build the project(signing and bundling are not supported yet)
[windows]
[linux]
build:
    conan build .

# Run static code analysis
sca:
    cppcheck --project=build/Release/compile_commands.json --enable=all --report-progress --check-config --suppress=missingIncludeSystem

# Runs plugin as a standalone app
[macos]
run:
    open build/Release/peakeater_artefacts/Release/Standalone/peakeater.app

# Package an application as a Conan package and test it with test project
package:
    conan export-pkg . -tf test
