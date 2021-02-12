#pragma once

#include <JuceHeader.h>

#include "../PluginProcessor.h"

#include "Label.h"

namespace widgets
{

//==============================================================================
class PeakAnalyzer : public juce::Component, public juce::Timer
{
public:
    //==============================================================================
    PeakAnalyzer(PeakEaterAudioProcessor& p):
        processor(p)
    {
        addAndMakeVisible(inputMagnitudeLabel);
        addAndMakeVisible(inputMagnitudeValue);
        addAndMakeVisible(clippedMagnitudeLabel);
        addAndMakeVisible(clippedMagnitudeValue);
        addAndMakeVisible(outputMagnitudeLabel);
        addAndMakeVisible(outputMagnitudeValue);
        startTimer(100);
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
        
        grid.justifyContent = juce::Grid::JustifyContent::end;
        
        grid.templateRows = {
            Track (Fr (1))
        };
        
        grid.templateColumns = {
            Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1))
        };
        
        grid.items = {
            Item (inputMagnitudeLabel),   Item (inputMagnitudeValue),
            Item (clippedMagnitudeLabel), Item (clippedMagnitudeValue),
            Item (outputMagnitudeLabel),  Item (outputMagnitudeValue),
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    //==============================================================================
    void timerCallback() override
    {
        auto dbClipped = processor.getDecibelsClipped();
        
        if (ticks >= 10 * 3)
        {
            prevDbClipped = dbClipped;
            ticks = 0;
        } else
        {
            if (dbClipped > prevDbClipped)
            {
                prevDbClipped = dbClipped;
            }
            ticks++;
        }
        
        inputMagnitudeValue.setText (dbToString (processor.getDecibelsIn()), juce::dontSendNotification);
        outputMagnitudeValue.setText (dbToString (processor.getDecibelsOut()), juce::dontSendNotification);
        clippedMagnitudeValue.setText (dbToString (prevDbClipped), juce::dontSendNotification);
    }
private:
    //==============================================================================
    PeakEaterAudioProcessor& processor;
    
    //==============================================================================
    unsigned int ticks  = 0;
    float prevDbClipped = 0;
    
    //==============================================================================
    widgets::Label inputMagnitudeLabel { "InputMagnitudeLabel", "IN:" };
    widgets::Label inputMagnitudeValue { "InputMagnitudeValue", "0 dB" };
    
    widgets::Label clippedMagnitudeLabel { "ClippedMagnitudeLabel", "EATEN:" };
    widgets::Label clippedMagnitudeValue { "ClippedMagnitudeValue", "0 dB" };
    
    widgets::Label outputMagnitudeLabel { "OutputMagnitudeLabel", "OUT:" };
    widgets::Label outputMagnitudeValue { "OutputMagnitudeValue", "0 dB" };
    
    //==============================================================================
    juce::String dbToString(const float db) const noexcept
    {
        return juce::String (std::roundf (db * 100) / 100) + " dB";
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakAnalyzer)
};

}
