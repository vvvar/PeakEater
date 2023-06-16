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

[macos]
[private]
setup-system:
    brew update
    brew bundle install --file=config/system/requirements.macos.brew.rb --no-lock

[windows]
[private]
setup-system:
    chocolatey install config/system/requirements.windows.choco.config --ignore-package-exit-codes

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

# Setup project's dependencies
setup: cleanup
    just setup-system
    just setup-git
    just setup-conan

# Build project
build:
    conan build . 

# Run static code analysis
sca:
    cppcheck --project=build/Release/compile_commands.json --enable=all --report-progress --check-config --suppress=missingIncludeSystem

# Runs plugin as a standalone app
[macos]
run:
    open build/Release/peakeater_artefacts/Release/Standalone/peakeater.app

# Sign & bundle an application as a Conan package
[macos]
package:
    conan export-pkg . -o signed=True -tf test

# Sign & bundle an application as a Conan package
[windows]
package:
    conan export-pkg . -o -tf test

# Sign & bundle an application as a Conan package
[linux]
package:
    conan export-pkg . -o -tf test
