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
    valueTreeState (vts)
{
    addAndMakeVisible (logo);
    addAndMakeVisible(logo_placeholder);
    juce::Image logoImage = juce::ImageCache::getFromMemory (BinaryData::logo_full_png, BinaryData::logo_full_pngSize);
    logo.setImages(false, true, true, logoImage, 1.0f, {}, logoImage, 1.0f, {}, logoImage, 1.0f, {});
    
    inputGainLabel.setText (Parameters::InputGain.Label, juce::dontSendNotification);
    addAndMakeVisible (inputGainLabel);
    addAndMakeVisible (inputGainSlider);
    inputGainAttachment.reset (new SliderAttachment (valueTreeState, Parameters::InputGain.Id, inputGainSlider));
    
    outputGainLabel.setText (Parameters::OutputGain.Label, juce::dontSendNotification);
    addAndMakeVisible (outputGainLabel);
    addAndMakeVisible (outputGainSlider);
    outputGainAttachment.reset (new SliderAttachment (valueTreeState, Parameters::OutputGain.Id, outputGainSlider));
    
    linkInOutLabel.setText (Parameters::LinkInOut.Label, juce::dontSendNotification);
    addAndMakeVisible (linkInOutLabel);
    addAndMakeVisible (linkInOutToggle);
    linkInOutAttachment.reset (new ButtonAttachment (valueTreeState, Parameters::LinkInOut.Id, linkInOutToggle));
    
    ceilingLabel.setText (Parameters::Ceiling.Label, juce::dontSendNotification);
    addAndMakeVisible (ceilingLabel);
    addAndMakeVisible (ceilingSlider);
    ceilingAttachment.reset (new SliderAttachment (valueTreeState, Parameters::Ceiling.Id, ceilingSlider));
    
    clippingTypeLabel.setText (Parameters::ClippingType.Label, juce::dontSendNotification);
    addAndMakeVisible (clippingTypeLabel);
    addAndMakeVisible (clippingTypeSlider);
    clippingTypeAttachment.reset (new SliderAttachment (valueTreeState, Parameters::ClippingType.Id, clippingTypeSlider));
    
    oversampleRateLabel.setText (Parameters::OversampleRate.Label, juce::dontSendNotification);
    addAndMakeVisible (oversampleRateLabel);
    addAndMakeVisible (oversampleRateSlider);
    oversampleRateAttachment.reset (new SliderAttachment (valueTreeState, Parameters::OversampleRate.Id, oversampleRateSlider));
    
    getLookAndFeel().setColour (juce::Slider::thumbColourId, AppColors::Red);
    getLookAndFeel().setColour (juce::Slider::trackColourId, AppColors::Navy);
    getLookAndFeel().setColour (juce::Slider::backgroundColourId, AppColors::Blue);
    getLookAndFeel().setColour (juce::Slider::textBoxOutlineColourId, AppColors::Blue);
    getLookAndFeel().setColour (juce::Slider::textBoxBackgroundColourId, AppColors::Paper);
    getLookAndFeel().setColour (juce::Slider::textBoxHighlightColourId, AppColors::Navy);
    getLookAndFeel().setColour (juce::ToggleButton::tickColourId, AppColors::Navy);
    getLookAndFeel().setColour (juce::ToggleButton::tickDisabledColourId, AppColors::Navy);
    getLookAndFeel().setColour (juce::Label::textColourId, AppColors::Navy);
    
    linkInOutToggle.addListener(this);

    inputGainSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
    outputGainSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
    ceilingSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
    clippingTypeSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
    oversampleRateSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
    
    setSize (400, 300);
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
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1))
    };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (3)) };
    
    grid.items = {
        Item (logo_placeholder), Item (logo).withMargin (Item::Margin (10, 0, 0, 130)),
        Item (inputGainLabel), Item (inputGainSlider),
        Item (outputGainLabel), Item (outputGainSlider),
        Item (linkInOutLabel), Item (linkInOutToggle),
        Item (ceilingLabel), Item (ceilingSlider),
        Item (clippingTypeLabel), Item (clippingTypeSlider),
        Item (oversampleRateLabel), Item (oversampleRateSlider)
    };
     
    grid.performLayout (getLocalBounds());
}

//==============================================================================
void MultiShaperAudioProcessorEditor::buttonClicked (juce::Button*)
{}

void MultiShaperAudioProcessorEditor::buttonStateChanged (juce::Button* button)
{
    if (button->getToggleState())
    {
        outputGainSlider.setEnabled(false);
        outputGainLabel.setColour (juce::Label::textColourId, AppColors::Blue);
        outputGainSlider.setColour (juce::Slider::thumbColourId, AppColors::Blue);
        outputGainSlider.setColour (juce::Slider::trackColourId, AppColors::Blue);
    } else
    {
        outputGainSlider.setEnabled(true);
        outputGainLabel.setColour (juce::Label::textColourId, AppColors::Navy);
        outputGainSlider.setColour (juce::Slider::thumbColourId, AppColors::Red);
        outputGainSlider.setColour (juce::Slider::trackColourId, AppColors::Navy);
    }
}
