#pragma once

#include <JuceHeader.h>

namespace layout
{

//==============================================================================
class Header : public juce::Component
{
public:
    //==============================================================================
    Header()
    {
        juce::Image logoImage = juce::ImageCache::getFromMemory (BinaryData::logo_full_png, BinaryData::logo_full_pngSize);
        logo.setImages(false, true, true, logoImage, 1.0f, {}, logoImage, 1.0f, {}, logoImage, 1.0f, {});
        
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
        grid.items = { Item(logo) };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    //==============================================================================
    juce::ImageButton logo;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Header)
};

}
