cmake_minimum_required(VERSION 3.15)
project(${CONAN_PROJECT_NAME} VERSION ${CONAN_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)

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
    Source/GUIv2/clipmeter/ClipMeter.cpp
    Source/GUIv2/editableparameter/EditableParameterComponentLookAndFeel.cpp
    Source/GUIv2/editableparameter/EditableLabelAttachment.cpp
    Source/GUIv2/editableparameter/EditableParameterComponent.cpp
    Source/GUIv2/dial/DialLookAndFeel.cpp
    Source/GUIv2/dial/Dial.cpp
    Source/GUIv2/dial/gaindial/GainDialLookAndFeel.cpp
    Source/GUIv2/dial/gaindial/GainDial.cpp
    Source/GUIv2/dial/ceilingdial/CeilingDial.cpp
    Source/GUIv2/dial/ceilingdial/CeilingDialLookAndFeel.cpp
    Source/GUIv2/linkinout/LinkInOut.cpp
    Source/GUIv2/bypass/BypassButton.cpp
    Source/GUIv2/scaling/ScalingSwitch.cpp
    Source/GUIv2/levelmeter/LevelMeterComponent.cpp
    Source/GUIv2/analyser/peakanalyzer/PeakMeter.cpp
    Source/GUIv2/analyser/peakanalyzer/PeakAnalyzerComponent.cpp
    Source/GUIv2/analyser/cliptype/ClipTypeComponent.cpp
    Source/GUIv2/analyser/AnalyserComponent.cpp
    Source/GUIv2/ControlPanel.cpp
    Source/GUIv2/LinkingPanel.cpp
    Source/GUIv2/LeftPanel.cpp
    Source/GUIv2/CentralPanel.cpp
    Source/GUIv2/RightPanel.cpp
    Source/GUIv2/Header.cpp
    Source/GUIv2/WorkingPanel.cpp
    Source/GUIv2/MainComponent.cpp
    Source/DSP/LevelMeter.cpp
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp)

target_compile_definitions(${CONAN_PROJECT_NAME}
    PUBLIC
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

juce_add_binary_data(AudioPluginData SOURCES
    Resources/link.png
    Resources/bypass.png
    Resources/icon.png
    Resources/logo_full.png
    Resources/logo_plugin.png
    Resources/WalkwayUpperBold.ttf)

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
