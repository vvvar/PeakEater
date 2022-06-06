/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace pe
{
//==============================================================================
PeakEaterAudioProcessorEditor::PeakEaterAudioProcessorEditor (PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : juce::AudioProcessorEditor (p)
    , main (p, vts)
{
    addAndMakeVisible (main);
    setSize (540, 350);
}

PeakEaterAudioProcessorEditor::~PeakEaterAudioProcessorEditor()
{
}

//==============================================================================
void PeakEaterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (pe::gui::AppColors::Paper);
}

void PeakEaterAudioProcessorEditor::resized()
{
    main.setBounds (getLocalBounds());
}
} // namespace pe
