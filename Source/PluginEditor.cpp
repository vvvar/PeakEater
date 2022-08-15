/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

namespace pe
{
//==============================================================================
PeakEaterAudioProcessorEditor::PeakEaterAudioProcessorEditor (PeakEaterAudioProcessor& audioProcessor,
                                                              std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                                                              std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                                                              std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                                                              std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
    : juce::AudioProcessorEditor (audioProcessor)
    , mMainComponent (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
{
    addAndMakeVisible (mMainComponent);
    setSize (820, 480);
}

PeakEaterAudioProcessorEditor::~PeakEaterAudioProcessorEditor()
{
}

//==============================================================================
void PeakEaterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll();
}

void PeakEaterAudioProcessorEditor::resized()
{
    mMainComponent.setBounds (getLocalBounds());
}
} // namespace pe
