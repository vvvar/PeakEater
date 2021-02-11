/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PeakEaterAudioProcessorEditor::PeakEaterAudioProcessorEditor (PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    :
    AudioProcessorEditor (&p),
    audioProcessor (p),
    valueTreeState (vts),
    main(p, vts)
{
    addAndMakeVisible(main);
    setSize (540, 350);
}

PeakEaterAudioProcessorEditor::~PeakEaterAudioProcessorEditor()
{}

//==============================================================================
void PeakEaterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (AppColors::Paper);
}

void PeakEaterAudioProcessorEditor::resized()
{

    main.setBounds(getLocalBounds());
}
