cmake_minimum_required(VERSION 3.15)
project(${CMAKE_PROJECT_NAME} VERSION ${CMAKE_PROJECT_VERSION})

# For this to work, the following ENV variables must be set:
# - MACOS_APPLE_DEVELOPER_ID: Apple Developer ID(for ex. "Developer ID Application: John Doe (XXXXX6XX42)")
option(CODESIGN "Codesign plugins and bundle(macOS only)" OFF)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(JUCE CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
    clap-juce-extensions
    GIT_REPOSITORY https://github.com/free-audio/clap-juce-extensions.git
    GIT_TAG b6cdda9f1fad06ba14e74e036f737d2ed11db2e7
)
FetchContent_MakeAvailable(clap-juce-extensions)

juce_add_plugin(${CMAKE_PROJECT_NAME}
    PLUGIN_MANUFACTURER_CODE Tado
    PLUGIN_CODE Tape
    LV2URI ${CONAN_PROJECT_URL}
    FORMATS AU VST3 LV2 Standalone
    VST3_CATEGORIES Fx Distortion Dynamics
    AU_MAIN_TYPE kAudioUnitType_Effect
    HARDENED_RUNTIME_ENABLED True
    COMPANY_NAME ${CONAN_PROJECT_COMPANY}
    PRODUCT_NAME ${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
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

target_compile_definitions(${CMAKE_PROJECT_NAME}
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

juce_generate_juce_header(${CMAKE_PROJECT_NAME})

clap_juce_extensions_plugin(TARGET ${CMAKE_PROJECT_NAME}
    CLAP_ID "com.T-Audio.peakeater"
    CLAP_FEATURES audio-effect)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    AudioPluginData # If we'd created a binary data target, we'd link to it here
    juce::juce_dsp
    juce::juce_audio_utils
    juce::juce_audio_plugin_client
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

if(APPLE)
    if(CODESIGN)
        if(NOT DEFINED ENV{MACOS_APPLE_DEVELOPER_ID})
            message(FATAL_ERROR "CODESIGN option is turned ON, but MACOS_APPLE_DEVELOPER_ID env variable is not defined")
        endif()

        # Codesign all plugins before we've bundled them
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_AU VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_AU> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning AU")
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_CLAP VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_CLAP> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning CLAP")
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_LV2 VERBATIM POST_BUILD # lv2 is special, sign only TARGET_FILE
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_FILE:${CMAKE_PROJECT_NAME}_LV2> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning LV2")
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_VST3 VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_VST3> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning VST3")
    endif()

    # Build DMG
    set(_OSX_DMG_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_artefacts/${CMAKE_BUILD_TYPE}/DMG")
    set(_OSX_DMG_FILE "${_OSX_DMG_DIR}/${CMAKE_PROJECT_NAME}.dmg")
    set_target_properties(${CMAKE_PROJECT_NAME}_AU ${CMAKE_PROJECT_NAME}_CLAP ${CMAKE_PROJECT_NAME}_LV2 ${CMAKE_PROJECT_NAME}_VST3 PROPERTIES BUNDLE TRUE) # otherwise dmg couldn't use TARGET_BUNDLE_DIR
    get_target_property(_LV2_OUT_DIR ${CMAKE_PROJECT_NAME}_LV2 LIBRARY_OUTPUT_DIRECTORY) # Getting it like this since lv2 folder is not a bundle, so TARGET_BUNDLE_DIR wont work
    add_custom_target(dmg ALL
        VERBATIM
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_OSX_DMG_DIR}
        COMMAND dmgbuild -s ${PROJECT_SOURCE_DIR}/config/dmgbuild.py -D au=$<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_AU> -D clap=$<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_CLAP> -D lv2=${_LV2_OUT_DIR} -D vst3=$<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_VST3> "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}" ${_OSX_DMG_FILE}
        COMMENT "Building DMG")
    add_dependencies(dmg ${CMAKE_PROJECT_NAME}_AU ${CMAKE_PROJECT_NAME}_CLAP ${CMAKE_PROJECT_NAME}_LV2 ${CMAKE_PROJECT_NAME}_VST3)

    if(CODESIGN)
        # Codesign, notarise and staple DMG
        add_custom_command(TARGET dmg VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v ${_OSX_DMG_FILE} --deep --strict --options=runtime --timestamp
            COMMAND xcrun notarytool submit ${_OSX_DMG_FILE} --keychain-profile "APPLE_SIGN_PROFILE" --wait
            COMMAND xcrun stapler staple ${_OSX_DMG_FILE}
            COMMENT "Codesigning, notarizing and stapling DMG")
    endif()
endif()

# Print available CMake targets for the debugging purpose
get_property(target_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
message(DEBUG " Available CMake targets are: ${target_names} ")