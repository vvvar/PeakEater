/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MultiShaperAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MultiShaperAudioProcessorEditor (MultiShaperAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~MultiShaperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    using SliderAttachment   = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment   = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    //==============================================================================
    MultiShaperAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    //==============================================================================
    juce::Label  inputGainLabel;
    juce::Slider inputGainSlider;
    std::unique_ptr<SliderAttachment> inputGainAttachment;
    
    juce::Label  outputGainLabel;
    juce::Slider outputGainSlider;
    std::unique_ptr<SliderAttachment> outputGainAttachment;
    
    juce::Label        linkInOutLabel;
    juce::ToggleButton linkInOutToggle;
    std::unique_ptr<ButtonAttachment> linkInOutAttachment;
    
    juce::Label  ceilingLabel;
    juce::Slider ceilingSlider;
    std::unique_ptr<SliderAttachment> ceilingAttachment;
    
    juce::Label  clippingTypeLabel;
    juce::Slider clippingTypeSlider;
    std::unique_ptr<SliderAttachment> clippingTypeAttachment;
    
    juce::Label  oversampleRateLabel;
    juce::Slider oversampleRateSlider;
    std::unique_ptr<SliderAttachment> oversampleRateAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiShaperAudioProcessorEditor)
};
