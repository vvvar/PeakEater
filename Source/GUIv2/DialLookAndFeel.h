#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
class DialLookAndFeel : public juce::LookAndFeel_V4
{
public:
    DialLookAndFeel();
    ~DialLookAndFeel() override;

    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;

    juce::Label* createSliderTextBox (juce::Slider& slider) override;
};
} // namespace gui
} // namespace pe