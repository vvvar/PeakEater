cmake_minimum_required(VERSION 3.15)
project(${CONAN_PROJECT_NAME} VERSION ${CONAN_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(JUCE CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
    clap-juce-extensions
    GIT_REPOSITORY https://github.com/free-audio/clap-juce-extensions.git
    GIT_TAG b6cdda9f1fad06ba14e74e036f737d2ed11db2e7
)
FetchContent_MakeAvailable(clap-juce-extensions)

juce_add_plugin(${CONAN_PROJECT_NAME}
    PLUGIN_MANUFACTURER_CODE Tado
    PLUGIN_CODE Tape
    LV2URI ${CONAN_PROJECT_URL}
    FORMATS AU VST3 LV2 Standalone
    VST3_CATEGORIES Fx Distortion Dynamics
    AU_MAIN_TYPE kAudioUnitType_Effect
    HARDENED_RUNTIME_ENABLED True
    COMPANY_NAME ${CONAN_PROJECT_COMPANY}
    PRODUCT_NAME ${CONAN_PROJECT_NAME})

target_sources(${CONAN_PROJECT_NAME}
    PRIVATE
    source/GUIv2/clipmeter/ClipMeter.cpp
    source/GUIv2/editableparameter/EditableParameterComponentLookAndFeel.cpp
    source/GUIv2/editableparameter/EditableLabelAttachment.cpp
    source/GUIv2/editableparameter/EditableParameterComponent.cpp
    source/GUIv2/dial/DialLookAndFeel.cpp
    source/GUIv2/dial/Dial.cpp
    source/GUIv2/dial/gaindial/GainDialLookAndFeel.cpp
    source/GUIv2/dial/gaindial/GainDial.cpp
    source/GUIv2/dial/ceilingdial/CeilingDial.cpp
    source/GUIv2/dial/ceilingdial/CeilingDialLookAndFeel.cpp
    source/GUIv2/linkinout/LinkInOut.cpp
    source/GUIv2/bypass/BypassButton.cpp
    source/GUIv2/scaling/ScalingSwitch.cpp
    source/GUIv2/levelmeter/LevelMeterComponent.cpp
    source/GUIv2/analyser/peakanalyzer/PeakMeter.cpp
    source/GUIv2/analyser/peakanalyzer/PeakAnalyzerComponent.cpp
    source/GUIv2/analyser/cliptype/ClipTypeComponent.cpp
    source/GUIv2/analyser/AnalyserComponent.cpp
    source/GUIv2/ControlPanel.cpp
    source/GUIv2/LinkingPanel.cpp
    source/GUIv2/LeftPanel.cpp
    source/GUIv2/CentralPanel.cpp
    source/GUIv2/RightPanel.cpp
    source/GUIv2/Header.cpp
    source/GUIv2/WorkingPanel.cpp
    source/GUIv2/MainComponent.cpp
    source/DSP/LevelMeter.cpp
    source/PluginEditor.cpp
    source/PluginProcessor.cpp)

target_compile_definitions(${CONAN_PROJECT_NAME}
    PUBLIC
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(AudioPluginData SOURCES
    assets/link.png
    assets/bypass.png
    assets/icon.png
    assets/logo_full.png
    assets/logo_plugin.png
    assets/WalkwayUpperBold.ttf)

set_target_properties(AudioPluginData PROPERTIES POSITION_INDEPENDENT_CODE ON)

juce_generate_juce_header(${CONAN_PROJECT_NAME})

clap_juce_extensions_plugin(TARGET ${CONAN_PROJECT_NAME}
    CLAP_ID "com.T-Audio.peakeater"
    CLAP_FEATURES audio-effect)

target_link_libraries(${CONAN_PROJECT_NAME}
    PRIVATE
    AudioPluginData # If we'd created a binary data target, we'd link to it here
    juce::juce_dsp
    juce::juce_audio_utils
    juce::juce_audio_plugin_client
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
