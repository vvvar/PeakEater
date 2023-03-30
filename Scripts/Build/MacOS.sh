# Fail-fast mode
set -euo pipefail

# Validate arguments
if [ -z "$1" ]
  then
    echo "Build type is not provided. Please pass it as first argument(e.g. Release, Debug)"
    exit 1
fi
if [ -z "$2" ]
  then
    echo "Build target is not provided. Please pass it as second argument(e.g. PeakEater_VST3, PeakEater_AU, PeakEater_LV2, PeakEater_CLAP, all)"
    exit 1
fi

echo "=========== macOS ==========="
echo "Build type: $1"
echo "Build target: $2"
echo "============================="

CMAKE_BUILD_TYPE=$1
CMAKE_BUILD_TARGET=$2

# Print all commands
set -x

# Configure
cmake . -B build \
        -G 'Ninja' \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
        -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
        -DJUCE_BUILD_EXAMPLES=OFF -DJUCE_BUILD_EXTRAS=ON \
        -DCMAKE_OSX_ARCHITECTURES='x86_64;arm64' \
        -DCMAKE_OSX_DEPLOYMENT_TARGET='10.9' \

# Build  
cmake --build build --config $CMAKE_BUILD_TYPE --target $CMAKE_BUILD_TARGET