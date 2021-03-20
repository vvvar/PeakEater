#pragma once

#include <JuceHeader.h>
#include "Clipper.h"
#include "Master.h"

namespace layout
{

//==============================================================================
class WorkingArea : public juce::Component
{
public:
    //==============================================================================
    WorkingArea(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
    clipper (p.getCeilingMeterSource(), vts),
    master (vts)
    {
        addAndMakeVisible (clipper);
        addAndMakeVisible (master);
    }
    
    //==============================================================================
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
        grid.templateColumns = { Track (Fr (5)), Track (Fr (3)) };
        grid.items = {
            Item(clipper), Item(master)
        };
         
        grid.performLayout (getLocalBounds());
    }
private:
    //==============================================================================
    layout::Clipper clipper;
    layout::Master  master;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorkingArea)
};

}
