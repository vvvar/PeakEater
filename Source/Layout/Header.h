#pragma once

#include <JuceHeader.h>

namespace layout
{

//==============================================================================
class Header : public juce::Component,
public juce::Button::Listener
{
public:
    //==============================================================================
    Header()
    {
        juce::Image logoImage = juce::ImageCache::getFromMemory (BinaryData::logo_plugin_png, BinaryData::logo_plugin_pngSize);
        logo.setImages (false, true, true, logoImage, 1.0f, {}, logoImage, 1.0f, {}, logoImage, 1.0f, {});
        logo.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        logo.addListener (this);
        addAndMakeVisible (logo);
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
        grid.items = { Item (logo).withMargin (Item::Margin (5, 27, 0, 27)) };
         
        grid.performLayout (getLocalBounds());
    }
    
    void buttonClicked (juce::Button*) override
    {
        github.launchInDefaultBrowser();
    }
    
private:
    //==============================================================================
    juce::ImageButton logo;
    
    //==============================================================================
    juce::URL github { "https://github.com/vvvar/PeakEater" };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Header)
};

}
