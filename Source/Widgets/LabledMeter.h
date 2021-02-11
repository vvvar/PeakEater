#pragma once

#include "../AppColours.h"

#include "Label.h"

namespace widgets
{

//==============================================================================
class LabledMeter : public juce::Component
{
public:
    //==============================================================================
    class LabledMeterLnf : public foleys::LevelMeterLookAndFeel
    {
    public:
        //==============================================================================
        LabledMeterLnf()
        {
            foleys::LevelMeterLookAndFeel();
            setColour (LevelMeter::lmTextColour,             AppColors::Paper);
            setColour (LevelMeter::lmTicksColour,            AppColors::Navy);
            setColour (LevelMeter::lmBackgroundColour,       AppColors::Paper);
            setColour (LevelMeter::lmMeterOutlineColour,     AppColors::Paper);
            setColour (LevelMeter::lmMeterBackgroundColour,  AppColors::Grey);
            setColour (LevelMeter::lmMeterGradientLowColour, AppColors::Green);
            setColour (LevelMeter::lmMeterGradientMidColour, AppColors::Green2);
            setColour (LevelMeter::lmMeterGradientMaxColour, AppColors::Red);
        }
    private:
        //==============================================================================
        using LevelMeter = foleys::LevelMeter;
        
        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabledMeterLnf)
    };
    
    LabledMeter(juce::String labelText, foleys::LevelMeterSource& meterSource)
    : label(labelText + "Label", labelText)
    {
        meter.setLookAndFeel (&meterLookAndFeel);
        meter.setMeterSource (&meterSource);
        addAndMakeVisible (meter);
        addAndMakeVisible (label);
    }
    
    ~LabledMeter()
    {
        meter.setLookAndFeel (nullptr);
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        juce::Grid grid;
         
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        
        grid.templateRows = {
            Track (Fr (1)),
            Track (Fr (18))
        };
        grid.templateColumns = { Track (Fr (1)) };
        
        grid.items = {
            Item (label),
            Item (meter)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    //==============================================================================
    using LevelMeter = foleys::LevelMeter;
    
    //==============================================================================
    widgets::Label label;
    LevelMeter     meter { LevelMeter::Default };
    LabledMeterLnf meterLookAndFeel;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabledMeter)
};

}
