#pragma once

#include "../AppColours.h"

#include "Label.h"

namespace pe
{
namespace gui
{
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
            setColour (LevelMeter::lmTextColour, AppColors::Green2);
            setColour (LevelMeter::lmTextClipColour, AppColors::Red);
            setColour (LevelMeter::lmTicksColour, AppColors::Navy);
            setColour (LevelMeter::lmBackgroundColour, AppColors::Paper);
            setColour (LevelMeter::lmMeterOutlineColour, AppColors::Blue);
            setColour (LevelMeter::lmMeterBackgroundColour, AppColors::Blue);
            setColour (LevelMeter::lmMeterGradientLowColour, AppColors::Green);
            setColour (LevelMeter::lmMeterGradientMidColour, AppColors::Green2);
            setColour (LevelMeter::lmMeterGradientMaxColour, AppColors::Red);
        }

        //==============================================================================
        void drawMaxNumber (juce::Graphics& g,
                            foleys::LevelMeter::MeterFlags meterType,
                            juce::Rectangle<float> bounds,
                            float maxGain) override
        {
            juce::ignoreUnused (meterType);

            g.setColour (AppColors::Paper);
            g.fillRect (bounds);
            const float maxDb = juce::Decibels::gainToDecibels (maxGain, -100.0f);
            g.setColour (findColour (maxDb > 0.0 ? foleys::LevelMeter::lmTextClipColour : foleys::LevelMeter::lmTextColour));
            g.setFont (bounds.getHeight() * 0.4f);
            if (maxDb > -100.f) // if some input coming
            {
                g.drawFittedText (juce::String (maxDb, 1) + " dB",
                                  bounds.reduced (2.0).toNearestInt(),
                                  juce::Justification::centred,
                                  1);
            }
            else // draw placeholder
            {
                g.drawFittedText (juce::String ("-", 1) + " dB",
                                  bounds.reduced (2.0).toNearestInt(),
                                  juce::Justification::centred,
                                  1);
            }
            g.setColour (AppColors::Paper);
            g.drawRoundedRectangle (bounds, 1, 1.0);
        }

    private:
        //==============================================================================
        using LevelMeter = foleys::LevelMeter;

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabledMeterLnf)
    };

    LabledMeter (juce::String labelText, foleys::LevelMeterSource& meterSource)
        : label (labelText + "Label", labelText)
    {
        meter.setLookAndFeel (&meterLookAndFeel);
        meter.setMeterSource (&meterSource);
        addAndMakeVisible (meter);
        addAndMakeVisible (label);
    }

    ~LabledMeter() override
    {
        meter.setLookAndFeel (nullptr);
    }

    //==============================================================================
    void paint (juce::Graphics&) override
    {
    }

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
            Item (label).withMargin (Item::Margin (0, 0, 0, -17)),
            Item (meter)
        };

        grid.performLayout (getLocalBounds());
    }

private:
    //==============================================================================
    using LevelMeter = foleys::LevelMeter;

    //==============================================================================
    widgets::Label label;
    LevelMeter meter{ LevelMeter::Default };
    LabledMeterLnf meterLookAndFeel;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabledMeter)
};
} // namespace widgets
} // namespace gui
} // namespace pe
