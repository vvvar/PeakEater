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
        
        grid.justifyContent = juce::Grid::JustifyContent::start;
        
        grid.templateRows = {
            Track (Fr (1))
        };
        
        grid.templateColumns = {
            Track (Fr (5)), Track (Fr (8)), Track (Fr (6)), Track (Fr (8)), Track (Fr (5)), Track (Fr (8))
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
        auto dbIn      = processor.getDecibelsIn();
        auto dbClipped = processor.getDecibelsClipped();
        auto dbOut     = processor.getDecibelsOut();
        
        if (ticks >= 10 * 3) // 3 seconds
        {
            prevDbIn      = dbIn;
            prevDbClipped = dbClipped;
            prevDbOut     = dbOut;
            ticks = 0;
        } else
        {
            if (dbIn > prevDbIn)
            {
                prevDbIn = dbIn;
            }
            if (dbClipped > prevDbClipped)
            {
                prevDbClipped = dbClipped;
            }
            if (dbOut > prevDbOut)
            {
                prevDbOut = dbOut;
            }
            ticks++;
        }
        
        setLabelValues(prevDbIn, prevDbClipped, prevDbOut);
    }
private:
    //==============================================================================
    PeakEaterAudioProcessor& processor;
    
    //==============================================================================
    unsigned int ticks  = 0;
    float prevDbIn      = 0;
    float prevDbClipped = 0;
    float prevDbOut     = 0;
    
    //==============================================================================
    widgets::Label inputMagnitudeLabel { "InputMagnitudeLabel", "IN:" };
    widgets::Label inputMagnitudeValue { "InputMagnitudeValue", "0 dB" };
    
    widgets::Label clippedMagnitudeLabel { "ClippedMagnitudeLabel", "EATEN:" };
    widgets::Label clippedMagnitudeValue { "ClippedMagnitudeValue", "0 dB" };
    
    widgets::Label outputMagnitudeLabel { "OutputMagnitudeLabel", "OUT:" };
    widgets::Label outputMagnitudeValue { "OutputMagnitudeValue", "0 dB" };
    
    //==============================================================================
    void setLabelValues(float input, float clipped, float output)
    {
        inputMagnitudeValue.setText   (dbToString (input), juce::dontSendNotification);
        clippedMagnitudeValue.setText (dbToString (clipped), juce::dontSendNotification);
        outputMagnitudeValue.setText  (dbToString (output), juce::dontSendNotification);
    }
    
    //==============================================================================
    juce::String dbToString(const float db) const noexcept
    {
        return juce::String (std::roundf (db * 100) / 100) + " dB";
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakAnalyzer)
};

}
