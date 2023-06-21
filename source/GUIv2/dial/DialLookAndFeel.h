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
        void drawBorder (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider);
        void drawCircle (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider);
        void drawStick (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider);
    };
} // namespace gui
} // namespace pe