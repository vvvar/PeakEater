#include "CeilingDialLookAndFeel.h"
#include <juce_core/juce_core.h>

namespace pe
{
namespace gui
{
namespace
{
float smoothDbValue (float const& dB, juce::LinearSmoothedValue<float>& smoother)
{
    smoother.skip (1);
    if (dB < smoother.getCurrentValue())
    {
        smoother.setTargetValue (dB);
    }
    else
    {
        smoother.setCurrentAndTargetValue (dB);
    }
    return smoother.getCurrentValue();
}
} // namespace
CeilingDialLookAndFeel::CeilingDialLookAndFeel (std::shared_ptr<pe::dsp::LevelMeter<float>> inputMeter,
                                                std::shared_ptr<pe::dsp::LevelMeter<float>> clippingMeter)
    : mInputMeter (inputMeter)
    , mClippingMeter (clippingMeter)
    , mRange (-36.0f, 0.0f, 0.1f, 1.9f)
    , mSmoothedInput (inputMeter->getDecibels())
    , mSmoothedClipping (clippingMeter->getDecibels())
{
    auto const refreshRate = 20;
    mSmoothedInput.reset (refreshRate, 0.5f);
    mSmoothedClipping.reset (refreshRate, 0.5f);
}

void CeilingDialLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    DialLookAndFeel::drawRotarySlider (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
    drawProgress (g, smoothDbValue (mInputMeter->getDecibels(), mSmoothedInput), juce::Colour{ juce::Colours::aquamarine }, x, y, width, height, rotaryStartAngle, rotaryEndAngle);
    //  drawProgress (g, smoothDbValue (mClippingMeter->getDecibels(), mSmoothedClipping), juce::Colour{ juce::Colours::darkred }, x, y, width, height, rotaryStartAngle, rotaryEndAngle);
    drawStick (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
}

void CeilingDialLookAndFeel::drawProgress (juce::Graphics& g,
                                           float value,
                                           juce::Colour const&& colour,
                                           int x,
                                           int y,
                                           int width,
                                           int height,
                                           const float rotaryStartAngle,
                                           const float rotaryEndAngle)
{
    auto const bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto const radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto const lineW = radius * 0.085f;
    auto const arcRadius = radius - lineW * 1.6f;
    auto const toAngle = rotaryStartAngle + mRange.convertTo0to1 (value) * (rotaryEndAngle - rotaryStartAngle);

    // Draw bi-directional progress
    juce::Path valueArc;
    valueArc.addCentredArc (bounds.getCentreX(),
                            bounds.getCentreY(),
                            arcRadius,
                            arcRadius,
                            0.0f,
                            rotaryStartAngle,
                            toAngle,
                            true);
    g.setColour (colour);
    g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}
} // namespace gui
} // namespace pe