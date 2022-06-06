#pragma once

#include <JuceHeader.h>

#include "../../Parameters.h"
#include "../AppColours.h"

namespace pe
{
namespace gui
{
namespace widgets
{
class Slider : public juce::Slider
{
public:
    //==============================================================================
    class SliderLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        SliderLookAndFeel()
        {
            setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
            setColour (juce::Slider::textBoxOutlineColourId, AppColors::Blue.withAlpha (0.0f));
            setColour (juce::Slider::thumbColourId, AppColors::Red);
            setColour (juce::Slider::trackColourId, AppColors::Navy);
            setColour (juce::Slider::backgroundColourId, AppColors::Blue);
        }
        ~SliderLookAndFeel() override {}
        juce::Font getLabelFont (juce::Label& label) override
        {
            label.setJustificationType (juce::Justification::centred);
            return { 10.0f };
        }
    };

    //==============================================================================
    Slider (params::Parameter const& parameter, juce::AudioProcessorValueTreeState& vts)
        : juce::Slider()
    {
        setLookAndFeel (&lnf);
        attachment.reset (new SliderAttachment (vts, parameter.getId(), *this));
    }

    Slider (
        juce::Slider::SliderStyle style,
        juce::Slider::TextEntryBoxPosition textBoxPosition,
        params::Parameter const& parameter,
        juce::AudioProcessorValueTreeState& vts,
        juce::String tooltipText = "")
        : juce::Slider (style, textBoxPosition)
    {
        setLookAndFeel (&lnf);
        attachment.reset (new SliderAttachment (vts, parameter.getId(), *this));
        setTooltip (tooltipText);
    }

    ~Slider()
    {
        setLookAndFeel (nullptr);
    }

    void setEnabled (bool isEnabled)
    {
        juce::Slider::setEnabled (isEnabled);
        if (isEnabled)
        {
            setColour (juce::Slider::thumbColourId, AppColors::Red);
            setColour (juce::Slider::trackColourId, AppColors::Navy);
        }
        else
        {
            setColour (juce::Slider::thumbColourId, AppColors::Blue);
            setColour (juce::Slider::trackColourId, AppColors::Blue);
        }
    }

private:
    //==============================================================================
    SliderLookAndFeel lnf;

    //==============================================================================
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> attachment;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Slider)
};
} // namespace widgets
} // namespace gui
} // namespace pe
