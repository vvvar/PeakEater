#include "DialLookAndFeel.h"
#include "../Utils.h"

namespace pe
{
namespace gui
{
DialLookAndFeel::DialLookAndFeel()
{
}

DialLookAndFeel::~DialLookAndFeel()
{
}

juce::Slider::SliderLayout DialLookAndFeel::getSliderLayout (juce::Slider& slider)
{
    auto const localBounds = slider.getLocalBounds();
    juce::Slider::SliderLayout layout;
    layout.textBoxBounds = localBounds.withY (-1);
    layout.sliderBounds = localBounds;
    return layout;
}

void DialLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    drawBorder (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
    drawStick (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
}

void DialLookAndFeel::drawBorder (juce::Graphics& g, int x, int y, int width, int height, float, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    auto const bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto const radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto const lineW = radius * 0.085f;
    auto const arcRadius = radius - lineW * 1.6f;
    // Draw border
    juce::Path backgroundArc;
    backgroundArc.addCentredArc (bounds.getCentreX(),
                                 bounds.getCentreY(),
                                 arcRadius,
                                 arcRadius,
                                 0.0f,
                                 rotaryStartAngle,
                                 rotaryEndAngle,
                                 true);
    g.setColour (juce::Colours::grey);
    g.strokePath (backgroundArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}

void DialLookAndFeel::drawCircle (juce::Graphics& g, int x, int y, int width, int height, float, const float, const float, juce::Slider&)
{
    auto const bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    // Draw circle
    auto const circleRadius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.6f;
    g.setColour (juce::Colours::grey);
    g.fillEllipse (juce::Rectangle<float> (
        bounds.getCentreX() - circleRadius / 2,
        bounds.getCentreY() - circleRadius / 2,
        circleRadius,
        circleRadius));
}

void DialLookAndFeel::drawStick (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    auto const bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto const radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto const toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto const lineWidth = radius * 0.085f;
    // Draw stick
    juce::Path stick;
    stick.addRectangle (-lineWidth / 2, (-lineWidth / 2), lineWidth, radius - lineWidth + 1.0f);
    g.setColour (juce::Colours::grey);
    g.fillPath (stick, juce::AffineTransform::rotation (toAngle + juce::MathConstants<float>::pi).translated (bounds.getCentre()));
}
} // namespace gui
} // namespace pe