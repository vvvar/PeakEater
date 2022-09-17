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
    auto localBounds = slider.getLocalBounds();

    juce::Slider::SliderLayout layout;

    layout.textBoxBounds = localBounds.withY (-1);
    layout.sliderBounds = localBounds;

    return layout;
}

void DialLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    drawBorder (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
    // drawCircle (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
    drawStick (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);

    if (mValueLabel)
    {
        auto const fontSize = calculateTextSize (slider.getTopLevelComponent()->getBounds().getWidth(), slider.getTopLevelComponent()->getBounds().getHeight());
        // std::cout << "fontSize: " << fontSize << std::endl;
        mValueLabel->setFont (fontSize);
    }
}

juce::Label* DialLookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* label = new juce::Label;
    label->setJustificationType (juce::Justification::centredBottom);
    label->setColour (juce::Label::textColourId, slider.findColour (juce::Slider::textBoxTextColourId));
    label->setColour (juce::Label::textWhenEditingColourId, slider.findColour (juce::Slider::textBoxTextColourId));
    label->setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
    label->setInterceptsMouseClicks (false, false);
    label->setFont (17.0f);
    mValueLabel = label;
    return label;
}

void DialLookAndFeel::drawBorder (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto lineW = radius * 0.085f;
    auto arcRadius = radius - lineW * 1.6f;

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

void DialLookAndFeel::drawCircle (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.085f;
    auto arcRadius = radius - lineW * 1.6f;

    // Draw circle
    auto const circleRadius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.6f;
    g.setColour (juce::Colours::grey);
    g.fillEllipse (juce::Rectangle<float> (
        bounds.getCentreX() - circleRadius / 2,
        bounds.getCentreY() - circleRadius / 2,
        circleRadius,
        circleRadius));
}

void DialLookAndFeel::drawStick (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
    auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = radius * 0.085f;
    auto arcRadius = radius - lineW * 1.6f;

    // Draw stick
    juce::Path stick;
    auto stickWidth = lineW;
    // stick.addRectangle (-stickWidth / 2, (-stickWidth / 2) + 17.0f, stickWidth, (radius * 0.4f) + lineW);
    stick.addRectangle (-stickWidth / 2, (-stickWidth / 2), stickWidth, radius - stickWidth + 1.0f);
    // g.setColour (juce::Colours::white);
    g.setColour (juce::Colours::grey);
    g.fillPath (stick, juce::AffineTransform::rotation (toAngle + 3.14f).translated (bounds.getCentre()));
}
} // namespace gui
} // namespace pe