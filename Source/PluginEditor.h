/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "Layout/Main.h"

//==============================================================================
class PeakEaterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PeakEaterAudioProcessorEditor (PeakEaterAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~PeakEaterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    PeakEaterAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    //==============================================================================
    layout::MainWindow main;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakEaterAudioProcessorEditor)
};
