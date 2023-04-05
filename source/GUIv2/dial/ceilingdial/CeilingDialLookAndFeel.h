#pragma once

#include "../../../DSP/LevelMeter.h"
#include "../DialLookAndFeel.h"

namespace pe
{
namespace gui
{
class CeilingDialLookAndFeel : public DialLookAndFeel
{
public:
CeilingDialLookAndFeel (std::shared_ptr<pe::dsp::LevelMeter<float> > inputMeter,
                        std::shared_ptr<pe::dsp::LevelMeter<float> > clippingMeter);

void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
void drawProgress (juce::Graphics& g,
                   float value,
                   juce::Colour const& colour,
                   int x,
                   int y,
                   int width,
                   int height,
                   const float rotaryStartAngle,
                   const float rotaryEndAngle);
void drawProgress (juce::Graphics& g,
                   float value,
                   juce::Colour const&& colour,
                   int x,
                   int y,
                   int width,
                   int height,
                   const float rotaryStartAngle,
                   const float rotaryEndAngle);

private:
std::shared_ptr<pe::dsp::LevelMeter<float> > mInputMeter;
std::shared_ptr<pe::dsp::LevelMeter<float> > mClippingMeter;
juce::NormalisableRange<float> mRange;
juce::LinearSmoothedValue<float> mSmoothedInput;
juce::LinearSmoothedValue<float> mSmoothedClipping;
};
} // namespace gui
} // namespace pe