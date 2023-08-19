cmake_minimum_required(VERSION 3.15)
project(${CMAKE_PROJECT_NAME} VERSION ${CMAKE_PROJECT_VERSION})

include(FetchContent)
include(CTest)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(JUCE CONFIG REQUIRED)

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

    # DSP
    source/processor/Clipper.cpp
    source/processor/LevelMeter.cpp

    # GUI
    source/editor/clipmeter/ClipMeter.cpp
    source/editor/editableparameter/EditableParameterComponentLookAndFeel.cpp
    source/editor/editableparameter/EditableLabelAttachment.cpp
    source/editor/editableparameter/EditableParameterComponent.cpp
    source/editor/dial/DialLookAndFeel.cpp
    source/editor/dial/Dial.cpp
    source/editor/dial/gaindial/GainDialLookAndFeel.cpp
    source/editor/dial/gaindial/GainDial.cpp
    source/editor/dial/ceilingdial/CeilingDial.cpp
    source/editor/dial/ceilingdial/CeilingDialLookAndFeel.cpp
    source/editor/linkinout/LinkInOut.cpp
    source/editor/bypass/BypassButton.cpp
    source/editor/scaling/ScalingSwitch.cpp
    source/editor/levelmeter/LevelMeterComponent.cpp
    source/editor/analyser/peakanalyzer/PeakMeter.cpp
    source/editor/analyser/peakanalyzer/PeakAnalyzerComponent.cpp
    source/editor/analyser/cliptype/ClipTypeComponent.cpp
    source/editor/analyser/AnalyserComponent.cpp
    source/editor/ControlPanel.cpp
    source/editor/LinkingPanel.cpp
    source/editor/LeftPanel.cpp
    source/editor/CentralPanel.cpp
    source/editor/RightPanel.cpp
    source/editor/Header.cpp
    source/editor/WorkingPanel.cpp
    source/editor/MainComponent.cpp

    # Entry point
    source/PluginEditor.cpp
    source/PluginProcessor.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC source)

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

# Where to store the result of the Pluginval run
set(_PLUGINVAL_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_artefacts/${CMAKE_BUILD_TYPE}/Pluginval")

# Adding targets for testing, signing and packaging which are very platform-specific
# NOTE: only macOS supports signing at the moment
if(APPLE)
    # Test we haven't missed any compiler options. Makes sense only on macOS at the moment
    add_test(NAME CompileOptions COMMAND python3 ${PROJECT_SOURCE_DIR}/test/test_compile_options.py --compile_commands=${CMAKE_BINARY_DIR}/compile_commands.json)

    # Marks VST and AU as CFBundle. Without it, TARGET_BUNDLE_DIR won't work and we need it
    # to get paths to sign and package them
    set_target_properties(${CMAKE_PROJECT_NAME}_AU ${CMAKE_PROJECT_NAME}_VST3 PROPERTIES BUNDLE TRUE)

    # Test VST3 and AU with Pluginval
    add_test(NAME Pluginval_VST3
        COMMAND pluginval --strictness-level 10 --verbose --skip-gui-tests --validate-in-process --output-dir ${_PLUGINVAL_OUT_DIR} $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_VST3>)
    add_test(NAME Pluginval_AU
        COMMAND pluginval --strictness-level 10 --verbose --skip-gui-tests --validate-in-process --output-dir ${_PLUGINVAL_OUT_DIR} $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_AU>)

    # Optionally, codesign all plugins before we've bundled them(ensure it with tests)
    if(DEFINED ENV{MACOS_APPLE_DEVELOPER_ID})
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_AU VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_AU> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning AU")
        add_test(NAME Codesign_AU COMMAND codesign -dv --verify --verbose=4 $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_AU>)

        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_CLAP VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_CLAP> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning CLAP")
        add_test(NAME Codesign_CLAP COMMAND codesign -dv --verify --verbose=4 $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_CLAP>)

        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_LV2 VERBATIM POST_BUILD # lv2 is special, sign only TARGET_FILE
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_FILE:${CMAKE_PROJECT_NAME}_LV2> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning LV2")
        add_test(NAME Codesign_LV2 COMMAND codesign -dv --verify --verbose=4 $<TARGET_FILE:${CMAKE_PROJECT_NAME}_LV2>)

        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_VST3 VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_VST3> --deep --strict --options=runtime --timestamp
            COMMENT "Codesigning VST3")
        add_test(NAME Codesign_VST3 COMMAND codesign -dv --verify --verbose=4 $<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_VST3>)
    endif()

    # Build DMG
    set(_OSX_DMG_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_artefacts/${CMAKE_BUILD_TYPE}/DMG")
    set(_OSX_DMG_FILE "${_OSX_DMG_DIR}/${CMAKE_PROJECT_NAME}-v${CMAKE_PROJECT_VERSION}-macOS-universal.dmg")
    get_target_property(_LV2_OUT_DIR ${CMAKE_PROJECT_NAME}_LV2 LIBRARY_OUTPUT_DIRECTORY) # Getting it like this since lv2 folder is not a bundle, so TARGET_BUNDLE_DIR wont work
    add_custom_target(DMG ALL
        BYPRODUCTS ${_OSX_DMG_FILE}
        VERBATIM
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_OSX_DMG_DIR}
        COMMAND dmgbuild -s ${PROJECT_SOURCE_DIR}/config/dmgbuild.py -D au=$<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_AU> -D clap=$<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_CLAP> -D lv2=${_LV2_OUT_DIR} -D vst3=$<TARGET_BUNDLE_DIR:${CMAKE_PROJECT_NAME}_VST3> "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}" ${_OSX_DMG_FILE}
        COMMENT "Building DMG")
    add_dependencies(DMG ${CMAKE_PROJECT_NAME}_AU ${CMAKE_PROJECT_NAME}_CLAP ${CMAKE_PROJECT_NAME}_LV2 ${CMAKE_PROJECT_NAME}_VST3)

    # Optionally, codesign, notarize and stable the DMG(ensure it with test)
    if(DEFINED ENV{MACOS_APPLE_DEVELOPER_ID})
        add_custom_command(TARGET DMG VERBATIM POST_BUILD
            COMMAND codesign --force -s $ENV{MACOS_APPLE_DEVELOPER_ID} -v ${_OSX_DMG_FILE} --deep --strict --options=runtime --timestamp
            COMMAND xcrun notarytool submit ${_OSX_DMG_FILE} --keychain-profile "APPLE_SIGN_PROFILE" --wait
            COMMAND xcrun stapler staple ${_OSX_DMG_FILE}
            COMMENT "Codesigning, notarizing and stapling DMG")
        add_test(NAME Codesign_DMG COMMAND codesign -dv --verify --verbose=4 ${_OSX_DMG_FILE})
    endif()
else()
    # The way of getting path to .vst3 is tricky since actual bin is inside ARCHIVE_OUTPUT_DIRECTORY
    set(_VST3_BUNDLE "$<GENEX_EVAL:$<TARGET_PROPERTY:${CMAKE_PROJECT_NAME}_VST3,ARCHIVE_OUTPUT_DIRECTORY>>/${CMAKE_PROJECT_NAME}.vst3")

    # Getting it like this since lv2 folder is not a bundle, so TARGET_BUNDLE_DIR wont work
    get_target_property(_LV2_OUT_DIR ${CMAKE_PROJECT_NAME}_LV2 LIBRARY_OUTPUT_DIRECTORY)

    # Archive everything into ZIP
    set(_ARCHIVE_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_artefacts/${CMAKE_BUILD_TYPE}/Archive")
    set(_ARCHIVE_FILE "${_ARCHIVE_DIR}/${CMAKE_PROJECT_NAME}-v${CMAKE_PROJECT_VERSION}-${CMAKE_HOST_SYSTEM_NAME}-x86_64.zip")
    add_custom_target(archive ALL
        VERBATIM
        COMMAND ${CMAKE_COMMAND} -E make_directory ${_ARCHIVE_DIR}
        COMMAND ${CMAKE_COMMAND} -E tar cf ${_ARCHIVE_FILE} --format=zip $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}_CLAP> ${_LV2_OUT_DIR} ${_VST3_BUNDLE}
        COMMENT "Archiving everything")
    add_dependencies(archive ${CMAKE_PROJECT_NAME}_CLAP ${CMAKE_PROJECT_NAME}_LV2 ${CMAKE_PROJECT_NAME}_VST3)

    # Test VST3 with Pluginval(only non-macOS-specific format supported)
    # NOTE#1: Unlike macOS, TARGET_FILE_DIR is used because unlike macOS is a file here
    # NOTE#2: Only Windows supports GUI on a CI, thus check for it
    add_test(NAME Pluginval_VST3
        COMMAND pluginval --strictness-level 10 --verbose $<$<NOT:$<BOOL:${WIN32}>>:--skip-gui-tests> --validate-in-process --output-dir ${_PLUGINVAL_OUT_DIR} ${_VST3_BUNDLE})
endif()
