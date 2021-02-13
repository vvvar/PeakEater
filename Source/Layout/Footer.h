#pragma once

#include <JuceHeader.h>

#include "../Widgets/CheckBox.h"
#include "../Widgets/PeakAnalyzer.h"

namespace layout
{

//==============================================================================
class Footer : public juce::Component
{
public:
    Footer(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
        linkInOut(Parameters::LinkInOut, vts, "Link In/Out"),
        bypass(Parameters::Bypass, vts),
        peakAnalyzer(p)
    {
        addAndMakeVisible(linkInOut);
        addAndMakeVisible(bypass);
        addAndMakeVisible(peakAnalyzer);
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
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (3)) };
        grid.items = {
            Item(bypass).withMargin (Item::Margin (0, 0, 0, 20)), Item(linkInOut), Item(peakAnalyzer).withMargin (Item::Margin (0, 20, 0, 0)),
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    //==============================================================================
    void addLinkInOutListener(juce::Button::Listener* listener)
    {
        linkInOut.addListener(listener);
    }
    
    void addBypassListener(juce::Button::Listener* listener)
    {
        bypass.addListener(listener);
    }
    
    //==============================================================================
    void setLinkEnabled(bool isEnabled)
    {
        linkInOut.setEnabled (isEnabled);
    }
    
private:
    //==============================================================================
    widgets::CheckBox     linkInOut;
    widgets::CheckBox     bypass;
    widgets::PeakAnalyzer peakAnalyzer;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Footer)
};

}
