/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MultiShaperAudioProcessorEditor::MultiShaperAudioProcessorEditor (MultiShaperAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    :
    AudioProcessorEditor (&p),
    audioProcessor (p),
    valueTreeState (vts),
    main(p, vts)
{
    lnf.setColour (foleys::LevelMeter::lmTextColour,             AppColors::Paper);
    lnf.setColour (foleys::LevelMeter::lmTextClipColour,         juce::Colours::white);
    lnf.setColour (foleys::LevelMeter::lmTextDeactiveColour,     juce::Colours::darkgrey);
    lnf.setColour (foleys::LevelMeter::lmTicksColour,            AppColors::Navy);
    lnf.setColour (foleys::LevelMeter::lmOutlineColour,          juce::Colours::orange);
    lnf.setColour (foleys::LevelMeter::lmBackgroundColour,       AppColors::Paper);
    lnf.setColour (foleys::LevelMeter::lmBackgroundClipColour,   juce::Colours::red);
    lnf.setColour (foleys::LevelMeter::lmMeterForegroundColour,  juce::Colours::green);
    lnf.setColour (foleys::LevelMeter::lmMeterOutlineColour,     AppColors::Paper);
    lnf.setColour (foleys::LevelMeter::lmMeterBackgroundColour,  AppColors::Grey);
    lnf.setColour (foleys::LevelMeter::lmMeterMaxNormalColour,   juce::Colours::lightgrey);
    lnf.setColour (foleys::LevelMeter::lmMeterMaxWarnColour,     juce::Colours::orange);
    lnf.setColour (foleys::LevelMeter::lmMeterMaxOverColour,     juce::Colours::darkred);
    lnf.setColour (foleys::LevelMeter::lmMeterGradientLowColour, AppColors::Green);
    lnf.setColour (foleys::LevelMeter::lmMeterGradientMidColour, AppColors::Green2);
    lnf.setColour (foleys::LevelMeter::lmMeterGradientMaxColour, AppColors::Red);
    lnf.setColour (foleys::LevelMeter::lmMeterReductionColour,   juce::Colours::orange);
    inputMeter.setLookAndFeel(&lnf);
    outputMeter.setLookAndFeel(&lnf);
    
    inputMeter.setMeterSource (&audioProcessor.getInputMeterSource());
    outputMeter.setMeterSource (&audioProcessor.getOutputMeterSource());
    
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(main);
    addAndMakeVisible(outputMeter);
    
    setSize (640, 400);
}

MultiShaperAudioProcessorEditor::~MultiShaperAudioProcessorEditor()
{}

//==============================================================================
void MultiShaperAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (AppColors::Paper);
}

void MultiShaperAudioProcessorEditor::resized()
{
    juce::Grid grid;
     
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    
    grid.templateRows = {
        Track (Fr (1)),
    };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (6)), Track (Fr (1)) };
    
    grid.items = {
        Item (inputMeter), Item (main), Item (outputMeter)
    };
     
    grid.performLayout (getLocalBounds());
}
