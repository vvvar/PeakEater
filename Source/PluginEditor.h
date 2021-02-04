/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
namespace AppColors
{

static const juce::Colour Paper   { 250, 250, 250 };
static const juce::Colour Paper2  { 241, 241, 230 };
static const juce::Colour Red     { 210, 14, 44 };
static const juce::Colour Navy    { 0, 29, 45 };
static const juce::Colour Blue    { 156, 174, 188 };
static const juce::Colour Yellow  { 234, 153, 39 };
static const juce::Colour Pastele { 255, 235, 204 };

};

//==============================================================================
/**
*/
class MultiShaperAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    MultiShaperAudioProcessorEditor (MultiShaperAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~MultiShaperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void buttonClicked (juce::Button *) override;
    void buttonStateChanged (juce::Button *) override;

private:
    //==============================================================================
    using SliderAttachment   = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment   = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    //==============================================================================
    MultiShaperAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    //==============================================================================
    juce::ImageButton logo;
    juce::ImageButton logo_placeholder;
    
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
