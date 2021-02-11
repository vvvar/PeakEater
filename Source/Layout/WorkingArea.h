#pragma once

#include <JuceHeader.h>

#include "../Widgets/LabledSlider.h"
#include "../Widgets/LabledMeterSlider.h"

namespace layout
{

//==============================================================================
class WorkingArea : public juce::Component
{
public:
    //==============================================================================
    WorkingArea(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
        inputGain(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow, Parameters::InputGain, vts),
        outputGain(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow, Parameters::OutputGain, vts),
        ceiling(p.getCeilingMeterSource(), vts, Parameters::Ceiling),
        clippingType(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow, Parameters::ClippingType, vts),
        oversampleRate(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow, Parameters::OversampleRate, vts)
    {
        addAndMakeVisible (inputGain);
        addAndMakeVisible (outputGain);
        addAndMakeVisible (ceiling);
        addAndMakeVisible (clippingType);
        addAndMakeVisible (oversampleRate);
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
        grid.items = {
            Item(inputGain), Item(clippingType), Item(ceiling), Item(oversampleRate), Item(outputGain),
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    //==============================================================================
    void setOutputGainEnabled(bool isEnabled)
    {
        outputGain.setEnabled (isEnabled);
    }
    
    void setEnabled(bool isEnabled)
    {
        inputGain.setEnabled (isEnabled);
        outputGain.setEnabled (isEnabled);
        ceiling.setEnabled (isEnabled);
        clippingType.setEnabled (isEnabled);
        oversampleRate.setEnabled (isEnabled);
    }
    
private:
    //==============================================================================
    widgets::LabledSlider      inputGain;
    widgets::LabledSlider      outputGain;
    widgets::LabledMeterSlider ceiling;
    widgets::LabledSlider      clippingType;
    widgets::LabledSlider      oversampleRate;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorkingArea)
};

}
