#pragma once

#include <JuceHeader.h>

#include "../Widgets/BlockPanel.h"

#include "../Widgets/LabledSlider.h"
#include "../Widgets/LabledMeterSlider.h"

namespace layout
{

//==============================================================================
class WorkingArea : public juce::Component
{
public:
    //==============================================================================
    WorkingArea(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    {
        addAndMakeVisible (clipper);
        addAndMakeVisible (master);
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
        grid.templateColumns = { Track (Fr (3)), Track (Fr (2)) };
        grid.items = {
            Item(clipper), Item(master)
        };
         
        grid.performLayout (getLocalBounds());
    }
private:
    //==============================================================================
    widgets::BlockPanel clipper { "Clipper" };
    widgets::BlockPanel master  { "Master" };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorkingArea)
};

}
