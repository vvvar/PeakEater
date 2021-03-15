#pragma once

#include <JuceHeader.h>
#include "../Parameters.h"
#include "../Widgets/BlockPanel.h"
#include "../Widgets/LabledRotarySlider.h"

namespace layout
{

//==============================================================================
class LinkInOutButton : public juce::ToggleButton
{
public:
    //==============================================================================
    using Img = juce::Image;
    using Cache = juce::ImageCache;
    
    //==============================================================================
    LinkInOutButton()
    {
        Img off_normal = Cache::getFromMemory (BinaryData::link_grey_png, BinaryData::link_grey_pngSize);
        Img off_over = Cache::getFromMemory (BinaryData::link_white_png, BinaryData::link_white_pngSize);
        Img off_down = Cache::getFromMemory (BinaryData::link_dgrey_png, BinaryData::link_dgrey_pngSize);
        off.setImages (true, true, true, off_normal, 1.0f, {}, off_over, 1.0f, {}, off_down, 1.0f, {});
        
        Img on_normal = Cache::getFromMemory (BinaryData::link_green_png, BinaryData::link_green_pngSize);
        Img on_over = Cache::getFromMemory (BinaryData::link_green_png, BinaryData::link_green_pngSize);
        Img on_down = Cache::getFromMemory (BinaryData::link_green_png, BinaryData::link_green_pngSize);
        on.setImages (true, true, true, on_normal, 1.0f, {}, on_over, 1.0f, {}, on_down, 1.0f, {});
        
        addAndMakeVisible (off);
        addAndMakeVisible (on);
    }
    ~LinkInOutButton() {}
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void clicked () override
    {
        setToggleState (!getToggleState());
    }
    
    void resized() override
    {
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (1)),
        };
        grid.templateColumns = { Track (Fr (1)) };
        if (getToggleState())
        {
            grid.items = {
                Item(on)
            };
        } else
        {
            grid.items = {
                Item(off)
            };
        }
        
        grid.performLayout (getLocalBounds());
    }
private:
    juce::ImageButton on;
    juce::ImageButton off;
};


//==============================================================================
class Master : public widgets::BlockPanel
{
public:
    Master (juce::AudioProcessorValueTreeState& vts):
    widgets::BlockPanel ("Master"),
    input (Parameters::InputGain, vts),
    output (Parameters::OutputGain, vts)
    {
        addAndMakeVisible (input);
        addAndMakeVisible (linkInOut);
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
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
        grid.items = {
            Item(input), Item(linkInOut), Item(output)
        };
         
        grid.performLayout (getReducedBounds().toNearestInt());
    }
private:
    widgets::LabledRotarySlider input;
    LinkInOutButton linkInOut;
    widgets::LabledRotarySlider output;
};

}

