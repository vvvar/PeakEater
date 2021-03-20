#pragma once

#include <JuceHeader.h>
#include "../Widgets/BlockPanel.h"
#include "../Widgets/LabledRotarySlider.h"
#include "../Widgets/LabledMeterSlider.h"
#include "../Parameters.h"
#include "../WaveShaping/ClippingFunctions.h"

namespace layout
{

//==============================================================================
class WaveformDisplay : public juce::Component
{
public:
    typedef float (*TransformFunction) (float);
    
    //==============================================================================
    WaveformDisplay()
    {
        for (int i = 0; i < NumSamples; i++)
        {
            const auto sin = ClipAmmount * std::sin(1 * i * juce::MathConstants<float>::twoPi / NumSamples);
            sinTable.push_back (sin);
        }
    }
    
    WaveformDisplay (TransformFunction transformFunction)
    {
        for (int i = 0; i < NumSamples; i++)
        {
            const auto sin = ClipAmmount * std::sin(1 * i * juce::MathConstants<float>::twoPi / NumSamples);
            sinTable.push_back (transformFunction(sin));
        }
    }
    
    //==============================================================================
    void paint (juce::Graphics &g) override
    {
        // Draw background
        g.setColour (AppTheme::V1::OSCBackgroundColour);
        g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (BorderThickness), BorderRadius);
        
        // Draw wavetable
        calculateLinesPlot();
        g.setColour (AppTheme::V1::OSCWaveColour);
        g.fillPath (path);
        
        // Draw border
        g.setColour (AppTheme::V1::OSCBorderColour);
        g.drawRoundedRectangle (getLocalBounds().toFloat().reduced (BorderThickness), BorderRadius, BorderThickness);
    }
    
    //==============================================================================
private:
    //==============================================================================
    // Consts
    const int NumSamples = 64; // Size of sine wave table
    const float ClipAmmount = 2.0f;
    const float WaveWidth = 1.0f;
    const float BorderRadius = 4.0f;
    const float BorderThickness = 1.0f;
    
    //==============================================================================
    // Caches
    std::vector<float> sinTable;
    juce::Path path;
        
    void calculateLinesPlot()
    {
        if (path.isEmpty())
        {
            for (auto i = 1; i < sinTable.size(); i++)
            {
                const float fromX = i - 1;
                const float fromY = sinTable[i-1] * sinTable.size();

                const float toX = i;
                const float toY = sinTable[i] * sinTable.size();

                path.addLineSegment ({ { fromX, fromY }, { toX, toY } }, WaveWidth);
            }
            
            const auto bounds = getLocalBounds().toFloat().reduced(WaveWidth + BorderThickness);
            const float x = bounds.getX();
            const float y = bounds.getY() * 8.0f;
            const float width = bounds.getWidth();
            const float height = bounds.getHeight() / 2.0;
            path.scaleToFit (x, y, width, height, false);
        }
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};

//==============================================================================
class Clipper : public widgets::BlockPanel
{
public:
    Clipper(foleys::LevelMeterSource& meterSource,
            juce::AudioProcessorValueTreeState& vts):
    widgets::BlockPanel ("CLIPPER"),
    algo (Parameters::ClippingType, vts),
    ceiling (meterSource, vts, Parameters::Ceiling)
    {
        addAndMakeVisible (waveformDisplay);
        addAndMakeVisible (waveformDisplay2);
        addAndMakeVisible (waveformDisplay3);
        addAndMakeVisible (waveformDisplay4);
        addAndMakeVisible (waveformDisplay5);
        addAndMakeVisible (waveformDisplay6);
        addAndMakeVisible (waveformDisplay7);
        addAndMakeVisible (algo);
        addAndMakeVisible (ceiling);
    }
    
    void resized() override
    {
        widgets::BlockPanel::resized();
        // waveformDisplay.setBounds (getLocalBounds());
        
        juce::Grid grid;
         
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        
        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
        
        grid.items = {
            Item (waveformDisplay).withMargin (Item::Margin (10, 0, 10, 0)), Item (algo), Item (ceiling)
        };
         
        grid.performLayout (getReducedBounds().toNearestInt());
    }
    
private:
    WaveformDisplay waveformDisplay;
    WaveformDisplay waveformDisplay2 { waveshaping::hardclip };
    WaveformDisplay waveformDisplay3 { waveshaping::quintic };
    WaveformDisplay waveformDisplay4 { waveshaping::cubicBasic };
    WaveformDisplay waveformDisplay5 { waveshaping::algClip };
    WaveformDisplay waveformDisplay6 { waveshaping::tanclip };
    WaveformDisplay waveformDisplay7 { waveshaping::arcClip };
    
    widgets::LabledRotarySlider algo;
    
    widgets::LabledMeterSlider ceiling;
};

}
