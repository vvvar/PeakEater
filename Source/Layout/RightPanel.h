#pragma once

#include <JuceHeader.h>

namespace layout
{

//==============================================================================
class RightPanel : public juce::Component
{
public:
    //==============================================================================
    RightPanel(PeakEaterAudioProcessor& p):
        outputMeter("OUT", p.getOutputMeterSource())
    {
        addAndMakeVisible(outputMeter);
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

        grid.templateRows = { Track (Fr (1)) };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = { Item (outputMeter) };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    //==============================================================================
    widgets::LabledMeter outputMeter;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightPanel)
};

}
