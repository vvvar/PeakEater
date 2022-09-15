#pragma once

#include "../DialLookAndFeel.h"

namespace pe
{
namespace gui
{
class GainDialLookAndFeel : public DialLookAndFeel
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
    void drawProgress (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider);
};
} // namespace gui
} // namespace pe