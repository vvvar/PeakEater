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
     
    setSize (400, 300);
}

MultiShaperAudioProcessorEditor::~MultiShaperAudioProcessorEditor()
{
}

//==============================================================================
void MultiShaperAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
}

void MultiShaperAudioProcessorEditor::resized()
{
    juce::Grid grid;
     
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
     
    grid.templateRows = {
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1)),
        Track (Fr (1))
    };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (3)) };
     
    grid.items = {
        juce::GridItem (inputGainLabel), juce::GridItem (inputGainSlider),
        juce::GridItem (outputGainLabel), juce::GridItem (outputGainSlider),
        juce::GridItem (linkInOutLabel), juce::GridItem (linkInOutToggle),
        juce::GridItem (ceilingLabel), juce::GridItem (ceilingSlider),
        juce::GridItem (clippingTypeLabel), juce::GridItem (clippingTypeSlider),
        juce::GridItem (oversampleRateLabel), juce::GridItem (oversampleRateSlider)
    };
     
    grid.performLayout (getLocalBounds());
}
