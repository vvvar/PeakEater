/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"

namespace pe
{
//==============================================================================
PeakEaterAudioProcessorEditor::PeakEaterAudioProcessorEditor (PeakEaterAudioProcessor& audioProcessor, juce::AudioProcessorValueTreeState& vts)
    : juce::AudioProcessorEditor (audioProcessor)
    , mMainComponent (audioProcessor.getInputMeterSource())
{
    addAndMakeVisible (mMainComponent);
    setSize (540, 350);
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
