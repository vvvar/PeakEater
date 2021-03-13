#pragma once

#include <JuceHeader.h>

#include "Header.h"
#include "WorkingArea.h"

namespace layout
{

//==============================================================================
class CentralPanel : public juce::Component
{
public:
    //==============================================================================
    CentralPanel(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
    workingArea (p, vts)
    {
        addAndMakeVisible (header);
        addAndMakeVisible (workingArea);
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
            Track (Fr (1)),
            Track (Fr (8))
        };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = {
//            Item (header).withMargin (Item::Margin (15, 0, 0, 0)),
            Item (header).withMargin (Item::Margin (7, 0, 7, 0)),
            Item (workingArea)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    layout::Header      header;
    layout::WorkingArea workingArea;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentralPanel)
};


}
