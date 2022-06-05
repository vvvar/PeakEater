/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

#include "GUI/Main.h"

namespace pe
{
//==============================================================================
class PeakEaterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PeakEaterAudioProcessorEditor (PeakEaterAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~PeakEaterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    pe::gui::layout::MainWindow main;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakEaterAudioProcessorEditor)
};
} // namespace pe