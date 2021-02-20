#pragma once

#include <JuceHeader.h>

#include "Header.h"
#include "WorkingArea.h"
#include "Footer.h"

namespace layout
{

//==============================================================================
class CentralPanel : public juce::Component, public juce::Button::Listener
{
public:
    //==============================================================================
    CentralPanel(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
        workingArea(p, vts),
        footer(p, vts)
    {
        addAndMakeVisible(header);
        addAndMakeVisible(workingArea);
        addAndMakeVisible(footer);
        
        footer.addLinkInOutListener(this);
        footer.addBypassListener(this);
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
            Track (Fr (5)),
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = {
            Item (header).withMargin (Item::Margin (15, 0, 0, 0)),
            Item (workingArea).withMargin (Item::Margin (20, 0, 0, 0)),
            Item (footer).withMargin (Item::Margin (19, 0, 0, 0))
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    //==============================================================================
    void buttonClicked (juce::Button* button) override
    {
        if (button->getName() == "LinkInOut")
        {
            workingArea.setOutputGainEnabled (!button->getToggleState());
        }
        if (button->getName() == "Bypass")
        {
            workingArea.setEnabled (!button->getToggleState());
            footer.setLinkEnabled (!button->getToggleState());
        }
    }
    
    void buttonStateChanged (juce::Button*) override
    {}
    
private:
    layout::Header      header;
    layout::WorkingArea workingArea;
    layout::Footer      footer;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CentralPanel)
};


}
