#pragma once

#include <JuceHeader.h>

#include "Label.h"
#include "RotarySlider.h"

namespace widgets
{

//==============================================================================
class LabledRotarySlider : public juce::Component
{
public:
    //==============================================================================
    LabledRotarySlider(
                       const Parameters::ParameterInfo& parameter,
                       juce::AudioProcessorValueTreeState& vts,
                       juce::String tooltipText = ""):
    slider (parameter, vts, tooltipText),
    label (parameter.Id + "Label", parameter.Label)
    {
        addAndMakeVisible (slider);
        addAndMakeVisible (label);
    }
    
    void resized() override
    {
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (9)),
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = {
            Item(slider),
            Item(label)
        };
         
        grid.performLayout (getLocalBounds());
    }
private:
    //==============================================================================
    widgets::RotarySlider slider;
    widgets::Label        label;
};

}
