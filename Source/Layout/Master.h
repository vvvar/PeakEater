#pragma once

#include <JuceHeader.h>
#include "../Parameters.h"
#include "../Widgets/BlockPanel.h"
#include "../Widgets/RotarySlider.h"

namespace layout
{

class Master : public widgets::BlockPanel
{
public:
    Master (juce::AudioProcessorValueTreeState& vts):
    widgets::BlockPanel ("Master"),
    input (Parameters::InputGain, vts),
    output (Parameters::OutputGain, vts)
    {
        addAndMakeVisible (input);
        addAndMakeVisible (output);
    }
    
    //==============================================================================
    void resized() override
    {
        widgets::BlockPanel::resized();
        
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (1)),
            Track (Fr (1)),
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)) };
        grid.items = {
            Item(input), Item(output)
        };
         
        grid.performLayout (getReducedBounds().toNearestInt());
    }
private:
    widgets::RotarySlider input;
    widgets::RotarySlider output;
};

}

