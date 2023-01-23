cmake_minimum_required(VERSION 3.15)
project(PeakEater VERSION 0.4.1)
set(CMAKE_CXX_STANDARD 20)

# Used for backwards compatibility with older Windows versions(less then Win 10).
# Should be used only for legacy versions. Do not use it for new Windows version.
# See discussions:
# - https://forum.juce.com/t/vst-vst3-cant-be-loaded-in-other-machine/34452
# - https://forum.juce.com/t/cmake-windows-static-runtime-libs-issue/41219
option(STATIC_MSVC_RUNTIME_LINKING "Statically link MSVC runtime" OFF)
if ($CACHE{STATIC_MSVC_RUNTIME_LINKING} MATCHES ON)
    message(STATUS "Will statically link MSVC runtime")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_subdirectory(Dependencies/JUCE)
add_subdirectory(Dependencies/clap-juce-extensions EXCLUDE_FROM_ALL) 

juce_add_plugin(PeakEater
    PLUGIN_MANUFACTURER_CODE Tado
    PLUGIN_CODE Tape
    LV2URI "https://github.com/vvvar/PeakEater"
    FORMATS AU VST3 LV2 Standalone
    VST3_CATEGORIES Fx Distortion Dynamics
    AU_MAIN_TYPE kAudioUnitType_Effect
    HARDENED_RUNTIME_ENABLED True
    COMPANY_NAME "T-Audio"
    PRODUCT_NAME "PeakEater")

target_sources(PeakEater
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

target_compile_definitions(PeakEater
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

set_target_properties(AudioPluginData PROPERTIES
    POSITION_INDEPENDENT_CODE ON)

juce_generate_juce_header(PeakEater)

clap_juce_extensions_plugin(TARGET PeakEater
    CLAP_ID "com.T-Audio.peakeater"
    CLAP_FEATURES audio-effect)

target_link_libraries(PeakEater
    PRIVATE
        AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_dsp
        juce::juce_audio_utils
        juce::juce_audio_plugin_client
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
