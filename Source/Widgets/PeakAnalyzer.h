#pragma once

#include <JuceHeader.h>

#include "../PluginProcessor.h"

#include "Label.h"

namespace widgets
{

//==============================================================================
class PeakAnalyzer : public juce::Component,
public widgets::LabelListener,
public juce::Timer
{
public:
    //==============================================================================
    PeakAnalyzer(PeakEaterAudioProcessor& p):
        processor(p)
    {
        addAndMakeVisible (inputMagnitudeLabel);
        addAndMakeVisible (inputMagnitudeValue);
        addAndMakeVisible (clippedMagnitudeLabel);
        addAndMakeVisible (clippedMagnitudeValue);
        addAndMakeVisible (outputMagnitudeLabel);
        addAndMakeVisible (outputMagnitudeValue);
        
        inputMagnitudeLabel.onClick (this);
        inputMagnitudeValue.onClick (this);
        clippedMagnitudeLabel.onClick (this);
        clippedMagnitudeValue.onClick (this);
        outputMagnitudeLabel.onClick (this);
        outputMagnitudeValue.onClick (this);
        
        const juce::String inputTooltip ("Highest peak value before clipping. Click to drop.");
        const juce::String clippedTooltip ("Highest peak value after clipping. Click to drop.");
        const juce::String outputTooltip ("Highest peak value after plugin. Click to drop.");
        
        inputMagnitudeLabel.setTooltip (inputTooltip);
        clippedMagnitudeLabel.setTooltip (clippedTooltip);
        outputMagnitudeLabel.setTooltip (outputTooltip);
        
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
        
        setLabelValues (prevDbIn, prevDbClipped, prevDbOut);
    }
    
    //==============================================================================
    void onClick(const juce::MouseEvent&) override
    {
        prevDbIn      = 0;
        prevDbClipped = 0;
        prevDbOut     = 0;
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
        
        if (input <= 0.0f)
        {
            inputMagnitudeValue.setColour (juce::Label::textColourId, AppColors::Green2);
        } else
        {
            inputMagnitudeValue.setColour (juce::Label::textColourId, AppColors::Red);
        }
        
        if (clipped < 12.0f)
        {
            clippedMagnitudeValue.setColour (juce::Label::textColourId, AppColors::Green2);
        } else
        {
            clippedMagnitudeValue.setColour (juce::Label::textColourId, AppColors::Red);
        }
        
        if (output <= 0.0f)
        {
            outputMagnitudeValue.setColour (juce::Label::textColourId, AppColors::Green2);
        } else
        {
            outputMagnitudeValue.setColour (juce::Label::textColourId, AppColors::Red);
        }
    }
    
    //==============================================================================
    juce::String dbToString(const float db) const noexcept
    {
        return juce::String (std::roundf (db * 10) / 10) + " dB";
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeakAnalyzer)
};

}
