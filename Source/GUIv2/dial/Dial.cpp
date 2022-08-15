#include "Dial.h"

#include <juce_graphics/juce_graphics.h>

namespace pe
{
namespace gui
{
Dial::Dial()
    : mLookAndFeel()
{
    setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    // setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
    setColour (juce::Slider::textBoxTextColourId, juce::Colours::black);
    setLookAndFeel (&mLookAndFeel);
    setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
                         juce::MathConstants<float>::pi * 2.75f,
                         true);
    setColour (juce::Slider::textBoxTextColourId, juce::Colours::antiquewhite);
    setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
    setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::blue);
    setVelocityBasedMode (true);
    setVelocityModeParameters (1.0, 1, 0.1, false);
    setRange (0.0, 100.0, 0.01);
    setValue (50.0);
    setDoubleClickReturnValue (true, 50.0);
    setTextValueSuffix (" %");
    onValueChange = [&]()
    {
        if (getValue() < 10)
            setNumDecimalPlacesToDisplay (2);
        else if (10 <= getValue() && getValue() < 100)
            setNumDecimalPlacesToDisplay (1);
        else
            setNumDecimalPlacesToDisplay (0);
    };
}

Dial::~Dial()
{
    setLookAndFeel (nullptr);
}
} // namespace gui
} // namespace pe