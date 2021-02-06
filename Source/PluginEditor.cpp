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
    addAndMakeVisible(main);
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
    main.setBounds(getLocalBounds());
}
