#pragma once

#include <JuceHeader.h>
#include "../Widgets/BlockPanel.h"

#include "../WaveShaping/ClippingFunctions.h"

namespace layout
{

//==============================================================================
class WaveformDisplay : public juce::Component
{
public:
    //==============================================================================
    WaveformDisplay ()
    {
        for (int i = 0; i < NumSamples; i++)
        {
            const auto sin = ClipAmmount * std::sin(1 * i * juce::MathConstants<float>::twoPi / NumSamples);
            sinTable.push_back (waveshaping::logiclip(sin));
        }
    }
    
    //==============================================================================
    void paint (juce::Graphics &g) override
    {
        g.setColour (AppTheme::V1::OSCWaveColour);
        calculateLinesPlot();
        g.fillPath (path);
    }
    
    //==============================================================================
private:
    //==============================================================================
    // Consts
    const int NumSamples = 512; // Size of sine wave table
    const float ClipAmmount = 2.0f;
    const float WaveWidth = 1.0f; // px
    
    //==============================================================================
    // Caches
    std::vector<float> sinTable;
    juce::Path path;
    
    void calculateLinesPlot()
    {
        if (path.isEmpty())
        {
            auto bounds = getLocalBounds().toFloat().reduced(WaveWidth);
            auto baseLeftX = bounds.getX();
            auto baseLeftY = bounds.getCentreY();
            auto widthMultiplier = bounds.getWidth() / sinTable.size();
            auto heightMultiplier = bounds.getHeight() / 2;
  
            for (auto i = 1; i < sinTable.size(); i++)
            {
                const auto fromX = (baseLeftX + (i - 1)) * widthMultiplier;
                const auto fromY = baseLeftY + (sinTable[i-1] * heightMultiplier);
                
                const auto toX = (baseLeftX + i) * widthMultiplier;
                const auto toY = baseLeftY + (sinTable[i] * heightMultiplier);

                path.addLineSegment(juce::Line<float>(juce::Point<float>(fromX, fromY), juce::Point<float>(toX, toY)), WaveWidth);
            }
        }
    }
};

//==============================================================================
class Clipper : public widgets::BlockPanel
{
public:
    Clipper(): widgets::BlockPanel("CLIPPER")
    {
        addAndMakeVisible (waveformDisplay);
    }
    
    void resized() override
    {
        waveformDisplay.setBounds (getLocalBounds());
    }
    
private:
    WaveformDisplay waveformDisplay;
};

}
