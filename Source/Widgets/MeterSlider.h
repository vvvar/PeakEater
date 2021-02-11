#pragma once

#include <JuceHeader.h>

#include "../AppColours.h"

#include "Slider.h"

namespace widgets
{

//==============================================================================
class MeterSlider : public juce::Component
{
public:
    //==============================================================================
    class MeterLookAndFeel : public foleys::LevelMeterLookAndFeel
    {
    public:
        MeterLookAndFeel()
        {
            foleys::LevelMeterLookAndFeel();
            setColour (foleys::LevelMeter::lmBackgroundColour, AppColors::Paper.withAlpha(0.0f));
            setColour (foleys::LevelMeter::lmMeterOutlineColour, AppColors::Blue.withAlpha(0.0f));
            setColour (foleys::LevelMeter::lmMeterBackgroundColour, AppColors::Blue);
            setColour (foleys::LevelMeter::lmMeterGradientLowColour, AppColors::Green);
            setColour (foleys::LevelMeter::lmMeterGradientMidColour, AppColors::Green2);
            setColour (foleys::LevelMeter::lmMeterGradientMaxColour, AppColors::Red);
        }
        juce::Rectangle<float> getMeterTickmarksBounds (juce::Rectangle<float> bounds,
                                                        foleys::LevelMeter::MeterFlags meterType) const override
        {
            return {};
        }
        juce::Rectangle<float> getMeterClipIndicatorBounds (juce::Rectangle<float> bounds,
                                                            foleys::LevelMeter::MeterFlags meterType) const override
        {
            return {};
        }
        juce::Rectangle<float> getMeterMaxNumberBounds (juce::Rectangle<float> bounds,
                                                        foleys::LevelMeter::MeterFlags meterType) const override
        {
            return {};
        }
        void drawMeterBarBackground (juce::Graphics& g,
                                     foleys::LevelMeter::MeterFlags meterType,
                                     juce::Rectangle<float> bounds) override
        {
            juce::ignoreUnused (meterType);
            g.setColour (findColour (foleys::LevelMeter::lmMeterBackgroundColour));
            g.fillRoundedRectangle  (bounds, ROUNDING);

            g.setColour (findColour (foleys::LevelMeter::lmMeterOutlineColour));
            g.drawRoundedRectangle (bounds, ROUNDING, 1.0);
        }
    private:
        const float ROUNDING = 3.0f;
        juce::ColourGradient horizontalGradient;
        juce::ColourGradient verticalGradient;
    };
    
    class SliderLookAndFeel : public widgets::Slider::SliderLookAndFeel
    {
    public:
        SliderLookAndFeel()
        : widgets::Slider::SliderLookAndFeel()
        {
            setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
            setColour (juce::Slider::trackColourId, AppColors::Paper.withAlpha (0.0f));
            setColour (juce::Slider::backgroundColourId, AppColors::Paper.withAlpha (0.0f));
            setColour (juce::Slider::textBoxOutlineColourId, AppColors::Paper.withAlpha (0.0f));
        }
    };
    
    MeterSlider(
                foleys::LevelMeterSource& meterSource,
                juce::AudioProcessorValueTreeState& valueTreeState,
                const Parameters::ParameterInfo& parameter)
    : slider(juce::Slider::SliderStyle::LinearVertical, juce::Slider::TextEntryBoxPosition::TextBoxBelow, parameter, valueTreeState)
    {
        meter.setLookAndFeel (&mLnF);
        slider.setLookAndFeel (&sLnF);
        
        meter.setFixedNumChannels (1);
        meter.setMeterSource (&meterSource);
        
        addAndMakeVisible (meter);
        addAndMakeVisible (slider);
    }
    
    ~MeterSlider()
    {
        meter.setLookAndFeel(nullptr);
        slider.setLookAndFeel(nullptr);
        setLookAndFeel(nullptr);
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        meter.setBounds (37, 6, getWidth() - 69, getHeight() - 9);
        slider.setBounds (getLocalBounds());
    }
    
    void setEnabled(bool isEnabled)
    {
        meter.setEnabled (isEnabled);
        slider.setEnabled (isEnabled);
        slider.setColour (juce::Slider::trackColourId, AppColors::Paper.withAlpha (0.0f));
    }
private:
    //==============================================================================
    widgets::Slider    slider;
    foleys::LevelMeter meter { foleys::LevelMeter::Default };
    
    MeterLookAndFeel  mLnF;
    SliderLookAndFeel sLnF;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterSlider)
};

}
