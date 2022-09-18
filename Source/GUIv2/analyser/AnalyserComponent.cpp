#include "AnalyserComponent.h"
#include "../Utils.h"

namespace pe
{
namespace gui
{
namespace
{
// DSP configuration
int constexpr gAnalyzerUpdateTimeout = 30;
int constexpr gAnalyzerForceUpdateTimeout = 4000;
float constexpr gMinDbValInOut = -36.0f;
float constexpr gMinDbValEaten = 0.0f;
// GUI configuration
float constexpr gBorderWidth = 1.0f;
float constexpr gBorderRadius = 3.0f;
} // namespace
AnalyserComponent::AnalyserComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                                      std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                                      std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                                      std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
    : mInputLevelMeter (inputLevelMeter)
    , mClippingLevelMeter (clippingLevelMeter)
    , mOutputLevelMeter (outputLevelMeter)
    , mInputAnalyzer (gMinDbValInOut)
    , mOutputAnalyzer (gMinDbValInOut)
    , mEatenAnalyzer (gMinDbValEaten)
    , mAnalyzerUpdateTimer (std::bind (&AnalyserComponent::onAnalyzerUpdateTick, this))
    , mAnalyzerForceUpdateTimer (std::bind (&AnalyserComponent::onAnalyzerForceUpdateTick, this))
{
    setMouseCursor (juce::MouseCursor::PointingHandCursor);
    mAnalyzerUpdateTimer.startTimer (gAnalyzerUpdateTimeout);
    mAnalyzerForceUpdateTimer.startTimer (gAnalyzerForceUpdateTimeout);
}

AnalyserComponent::~AnalyserComponent()
{
    setLookAndFeel (nullptr);
}

void AnalyserComponent::resized()
{
}

void AnalyserComponent::paint (juce::Graphics& g)
{
    drawLevels (mInputAnalyzer.getNext(), mOutputAnalyzer.getNext(), mEatenAnalyzer.getNext(), g);
}

void AnalyserComponent::drawLevels (float inputLevel,
                                    float outputLevel,
                                    float eatenAmount,
                                    juce::Graphics& g)
{
    auto const bounds = getBounds().reduced (gBorderWidth).toFloat();
    auto const width = bounds.getWidth();
    auto const height = bounds.getHeight();

    int const poxX = 0;
    int const posY = 0;
    int const boxWidth = width;
    int const boxHeight = height;
    auto const fontSize = calculatePrimaryTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
    auto const newlinePadding = fontSize * 0.5f;

    juce::Colour borderColor (juce::Colours::grey);
    juce::Colour backgroundColor (juce::Colours::grey.withAlpha (0.4f));
    juce::Colour textColor (juce::Colours::white);
    if (! isEnabled())
    {
        borderColor = borderColor.withAlpha (0.5f);
        backgroundColor = backgroundColor.withAlpha (0.5f);
        textColor = textColor.withAlpha (0.5f);
    }

    const juce::Rectangle<float> area (poxX, posY, boxWidth, boxHeight);
    g.setColour (backgroundColor);
    g.fillRoundedRectangle (area, gBorderRadius);
    g.setColour (borderColor);
    g.drawRoundedRectangle (area, gBorderRadius, gBorderWidth);

    std::string const inputLevelText = "Input: " + gToStringWithPrecision (gRoundDb (inputLevel), 1) + " dB";
    std::string const outLevelText = "Output: " + gToStringWithPrecision (gRoundDb (outputLevel), 1) + " dB";
    std::string const eatenLevelText = "Eaten: " + gToStringWithPrecision (gRoundDb (eatenAmount), 1) + " dB";
    g.setFont (fontSize);
    g.setColour (textColor);
    g.drawText (inputLevelText, poxX + newlinePadding, posY + newlinePadding, boxWidth, fontSize, juce::Justification::left, true);
    g.drawText (outLevelText, poxX + newlinePadding, posY + newlinePadding + fontSize + newlinePadding, boxWidth, fontSize, juce::Justification::left, true);
    g.drawText (eatenLevelText, poxX + newlinePadding, posY + newlinePadding + (fontSize * 2) + (newlinePadding * 2), boxWidth, fontSize, juce::Justification::left, true);
}

void AnalyserComponent::mouseDown (juce::MouseEvent const& event)
{
    if (event.getNumberOfClicks() == 2)
    {
        mInputAnalyzer.reset();
        mOutputAnalyzer.reset();
        mEatenAnalyzer.reset();
        repaint();
    }
}

void AnalyserComponent::onAnalyzerUpdateTick()
{
    auto const inputLevel = static_cast<float> (mInputLevelMeter->getDecibels());
    auto const outputLevel = static_cast<float> (mOutputLevelMeter->getDecibels());
    auto const clippingLevel = static_cast<float> (mClippingLevelMeter->getDecibels());
    mInputAnalyzer.push (inputLevel);
    mOutputAnalyzer.push (outputLevel);
    mEatenAnalyzer.push (inputLevel - clippingLevel);
}

void AnalyserComponent::onAnalyzerForceUpdateTick()
{
    auto const inputLevel = static_cast<float> (mInputLevelMeter->getDecibels());
    auto const outputLevel = static_cast<float> (mOutputLevelMeter->getDecibels());
    auto const clippingLevel = static_cast<float> (mClippingLevelMeter->getDecibels());
    mInputAnalyzer.push (inputLevel, true);
    mOutputAnalyzer.push (outputLevel, true);
    mEatenAnalyzer.push (inputLevel - clippingLevel, true);
}
} // namespace gui
} // namespace pe