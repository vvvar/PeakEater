#include "PeakAnalyzerComponent.h"

#include <JuceHeader.h>

#include <cmath>

#include "../../ColourScheme.h"
#include "../../Utils.h"

namespace pe::gui {

namespace {
// DSP configuration
int constexpr gAnalyzerSampleRateHz = 30;
float constexpr gMinDbValInOut = -36.0f;
float constexpr gMinDbValEaten = 0.0f;

template <typename T>
bool gIsInBounds(const T& value, const T& low, const T& high) {
    return !(value < low) && (value < high);
}

std::string stringifyMagnitude(float const magnitude) {
    auto const precision = 1;
    return gToStringWithPrecision(gRoundDb(magnitude), precision) + " dB";
}
}  // namespace

PeakAnalyzerComponent::PeakAnalyzerComponent(LevelMetersPack const& levelMetersPack)
    : mInputLevelMeter(levelMetersPack.inputLevelMeter),
      mClippingLevelMeter(levelMetersPack.clippingLevelMeter),
      mOutputLevelMeter(levelMetersPack.outputLevelMeter),
      mInputPeakMeter(gAnalyzerSampleRateHz, gMinDbValInOut),
      mOutputPeakMeter(gAnalyzerSampleRateHz, gMinDbValInOut),
      mEatenPeakMeter(gAnalyzerSampleRateHz, gMinDbValEaten),
      mUpdateTimer(std::bind(&PeakAnalyzerComponent::onUpdateTick, this)) {
    mInputLabel.setJustificationType(juce::Justification::verticallyCentred);
    mOutputLabel.setJustificationType(juce::Justification::verticallyCentred);
    mEatenLabel.setJustificationType(juce::Justification::verticallyCentred);

    addAndMakeVisible(mInputLabel);
    addAndMakeVisible(mOutputLabel);
    addAndMakeVisible(mEatenLabel);
    mUpdateTimer.startTimerHz(gAnalyzerSampleRateHz);

    setMouseCursor(juce::MouseCursor::PointingHandCursor);
    mInputLabel.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    mOutputLabel.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    mEatenLabel.setMouseCursor(juce::MouseCursor::PointingHandCursor);

    addMouseListener(this, true);
}

PeakAnalyzerComponent::~PeakAnalyzerComponent() { setLookAndFeel(nullptr); }

void PeakAnalyzerComponent::resized() {
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1))};
    grid.items = {Item(mInputLabel), Item(mOutputLabel), Item(mEatenLabel)};

    auto const fontSize =
        calculatePrimaryTextSize(getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
    mInputLabel.setFont(fontSize);
    mOutputLabel.setFont(fontSize);
    mEatenLabel.setFont(fontSize);

    grid.performLayout(getLocalBounds());
}

void PeakAnalyzerComponent::paint(juce::Graphics&) {
    auto const inputAmount = mInputPeakMeter.getMaxPeak();
    auto const outputAmount = mOutputPeakMeter.getMaxPeak();
    auto eatenAmount = mEatenPeakMeter.getMaxPeak();
    if (eatenAmount < 0) {
        eatenAmount = 0.0f;
    }

    if (gIsInBounds(eatenAmount, 10.0f, 20.0f)) {
        mEatenLabel.setColour(juce::Label::textColourId, colourscheme::Warning);
    } else if (gIsInBounds(eatenAmount, 20.0f, std::numeric_limits<float>::max())) {
        mEatenLabel.setColour(juce::Label::textColourId, colourscheme::ForegroundSecondary);
    } else {
        mEatenLabel.setColour(juce::Label::textColourId, colourscheme::TextFocusLevel0);
    }

    mInputLabel.setText("Input: " + stringifyMagnitude(inputAmount), juce::NotificationType::dontSendNotification);
    mOutputLabel.setText("Output: " + stringifyMagnitude(outputAmount), juce::NotificationType::dontSendNotification);
    mEatenLabel.setText("Eaten: " + stringifyMagnitude(eatenAmount), juce::NotificationType::dontSendNotification);
}

void PeakAnalyzerComponent::mouseDown(juce::MouseEvent const& event) {
    auto const numClicksInDoubleClick = 2;
    if (numClicksInDoubleClick == event.getNumberOfClicks()) {
        mInputPeakMeter.reset();
        mOutputPeakMeter.reset();
        mEatenPeakMeter.reset();
    }
}

void PeakAnalyzerComponent::onUpdateTick() {
    auto const inputLevel = static_cast<float>(mInputLevelMeter->getDBFS());
    auto const outputLevel = static_cast<float>(mOutputLevelMeter->getDBFS());
    auto const clippingLevel = static_cast<float>(mClippingLevelMeter->getDBFS());
    mInputPeakMeter.push(inputLevel);
    mOutputPeakMeter.push(outputLevel);
    mEatenPeakMeter.push(inputLevel - clippingLevel);
}
}  // namespace pe::gui
