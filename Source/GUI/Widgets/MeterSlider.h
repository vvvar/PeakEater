#pragma once

#include <JuceHeader.h>

#include "../AppColours.h"

#include "Slider.h"

namespace pe
{
namespace gui
{
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
            setColour (foleys::LevelMeter::lmBackgroundColour, AppColors::Paper.withAlpha (0.0f));
            setColour (foleys::LevelMeter::lmMeterOutlineColour, AppColors::Blue.withAlpha (0.0f));
            setColour (foleys::LevelMeter::lmMeterBackgroundColour, AppColors::Blue);
            setColour (foleys::LevelMeter::lmMeterGradientLowColour, AppColors::Green);
            setColour (foleys::LevelMeter::lmMeterGradientMidColour, AppColors::Green2);
            setColour (foleys::LevelMeter::lmMeterGradientMaxColour, AppColors::Red);
        }
        juce::Rectangle<float> getMeterTickmarksBounds (juce::Rectangle<float> /* bounds */,
                                                        foleys::LevelMeter::MeterFlags /* meterType */) const override
        {
            return {};
        }
        juce::Rectangle<float> getMeterClipIndicatorBounds (juce::Rectangle<float> /* bounds */,
                                                            foleys::LevelMeter::MeterFlags /* meterType */) const override
        {
            return {};
        }
        juce::Rectangle<float> getMeterMaxNumberBounds (juce::Rectangle<float> /* bounds */,
                                                        foleys::LevelMeter::MeterFlags /* meterType */) const override
        {
            return {};
        }
        void drawMeterBarBackground (juce::Graphics& g,
                                     foleys::LevelMeter::MeterFlags meterType,
                                     juce::Rectangle<float> bounds) override
        {
            juce::ignoreUnused (meterType);
            g.setColour (findColour (foleys::LevelMeter::lmMeterBackgroundColour));
            g.fillRoundedRectangle (bounds, ROUNDING);

            g.setColour (findColour (foleys::LevelMeter::lmMeterOutlineColour));
            g.drawRoundedRectangle (bounds, ROUNDING, 1.0);
        }

        void drawMeterBar (juce::Graphics& g,
                           foleys::LevelMeter::MeterFlags meterType,
                           juce::Rectangle<float> bounds,
                           float rms,
                           float peak) override
        {
            const auto infinity = -36.0f;
            const auto rmsDb = juce::Decibels::gainToDecibels (rms, infinity);
            const auto peakDb = juce::Decibels::gainToDecibels (peak, infinity);

            const juce::Rectangle<float> floored (ceilf (bounds.getX()) + 1.0f, ceilf (bounds.getY()) + 1.0f, floorf (bounds.getRight()) - (ceilf (bounds.getX() + 2.0f)), floorf (bounds.getBottom()) - (ceilf (bounds.getY()) + 2.0f));

            if (meterType & foleys::LevelMeter::Vintage)
            {
                // TODO
            }
            else if (meterType & foleys::LevelMeter::Reduction)
            {
                const float limitDb = juce::Decibels::gainToDecibels (rms, infinity);
                g.setColour (findColour (foleys::LevelMeter::lmMeterReductionColour));
                if (meterType & foleys::LevelMeter::Horizontal)
                    g.fillRect (floored.withLeft (floored.getX() + limitDb * floored.getWidth() / infinity));
                else
                    g.fillRect (floored.withBottom (floored.getY() + limitDb * floored.getHeight() / infinity));
            }
            else
            {
                if (meterType & foleys::LevelMeter::Horizontal)
                {
                    if (horizontalGradient.getNumColours() < 2)
                    {
                        horizontalGradient = juce::ColourGradient (findColour (foleys::LevelMeter::lmMeterGradientLowColour),
                                                                   floored.getX(),
                                                                   floored.getY(),
                                                                   findColour (foleys::LevelMeter::lmMeterGradientMaxColour),
                                                                   floored.getRight(),
                                                                   floored.getY(),
                                                                   false);
                        horizontalGradient.addColour (0.5, findColour (foleys::LevelMeter::lmMeterGradientLowColour));
                        horizontalGradient.addColour (0.75, findColour (foleys::LevelMeter::lmMeterGradientMidColour));
                    }
                    g.setGradientFill (horizontalGradient);
                    g.fillRect (floored.withRight (floored.getRight() - rmsDb * floored.getWidth() / infinity));

                    if (peakDb > -49.0)
                    {
                        g.setColour (findColour ((peakDb > -0.3f) ? foleys::LevelMeter::lmMeterMaxOverColour : ((peakDb > -5.0) ? foleys::LevelMeter::lmMeterMaxWarnColour : foleys::LevelMeter::lmMeterMaxNormalColour)));
                        g.drawVerticalLine (juce::roundToInt (floored.getRight() - juce::jmax (peakDb * floored.getWidth() / infinity, 0.0f)),
                                            floored.getY(),
                                            floored.getBottom());
                    }
                }
                else
                {
                    // vertical
                    if (verticalGradient.getNumColours() < 2)
                    {
                        verticalGradient = juce::ColourGradient (findColour (foleys::LevelMeter::lmMeterGradientLowColour),
                                                                 floored.getX(),
                                                                 floored.getBottom(),
                                                                 findColour (foleys::LevelMeter::lmMeterGradientMaxColour),
                                                                 floored.getX(),
                                                                 floored.getY(),
                                                                 false);
                        verticalGradient.addColour (0.5f, findColour (foleys::LevelMeter::lmMeterGradientLowColour));
                        verticalGradient.addColour (0.75f, findColour (foleys::LevelMeter::lmMeterGradientMidColour));
                    }
                    g.setGradientFill (verticalGradient);

                    auto shouldSubstract = floored.getY() + floored.getHeight() * 1.3f * calculateOffset (0.0f, -36.0f, peakDb > 0 ? 0 : peakDb, 0.75f);
                    g.fillRect (floored.withTop (shouldSubstract));
                }
            }
        }

    private:
        const float ROUNDING = 3.0f;
        juce::ColourGradient horizontalGradient;
        juce::ColourGradient verticalGradient;

        float calculateOffset (float start, float end, float value, float skew)
        {
            auto proportion = (value - start) / (end - start);
            return std::pow (proportion, skew);
        }
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

    MeterSlider (
        foleys::LevelMeterSource& meterSource,
        juce::AudioProcessorValueTreeState& valueTreeState,
        params::Parameter const& parameter,
        juce::String sliderTooltip = "")
        : slider (juce::Slider::SliderStyle::LinearVertical,
                  juce::Slider::TextEntryBoxPosition::TextBoxBelow,
                  parameter,
                  valueTreeState,
                  sliderTooltip)
    {
        meter.setLookAndFeel (&mLnF);
        slider.setLookAndFeel (&sLnF);

        meter.setFixedNumChannels (1);
        meter.setMeterSource (&meterSource);

        addAndMakeVisible (meter);
        addAndMakeVisible (slider);
    }

    ~MeterSlider() override
    {
        meter.setLookAndFeel (nullptr);
        slider.setLookAndFeel (nullptr);
        setLookAndFeel (nullptr);
    }

    //==============================================================================
    void paint (juce::Graphics&) override
    {
    }

    void resized() override
    {
        meter.setBounds (37, 6, getWidth() - 69, getHeight() - 9);
        slider.setBounds (getLocalBounds());
    }

    void setEnabled (bool isEnabled)
    {
        meter.setEnabled (isEnabled);
        slider.setEnabled (isEnabled);
        slider.setColour (juce::Slider::trackColourId, AppColors::Paper.withAlpha (0.0f));
    }

private:
    //==============================================================================
    widgets::Slider slider;
    foleys::LevelMeter meter{ foleys::LevelMeter::Default };

    MeterLookAndFeel mLnF;
    SliderLookAndFeel sLnF;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterSlider)
};
} // namespace widgets
} // namespace gui
} // namespace pe
