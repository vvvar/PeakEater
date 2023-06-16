#include "PeakAnalyzerComponent.h"

#include <JuceHeader.h>
#include <cmath>

#include "../../ColourScheme.h"
#include "../../Utils.h"

namespace pe
{
namespace gui
{
    namespace
    {
        // DSP configuration
        int constexpr gAnalyzerSampleRateHz = 30;
        float constexpr gMinDbValInOut = -36.0f;
        float constexpr gMinDbValEaten = 0.0f;
        template <typename T>
        bool gIsInBounds (const T& value, const T& low, const T& high)
        {
            return ! (value < low) && (value < high);
        }
    } // namespace
    PeakAnalyzerComponent::PeakAnalyzerComponent (std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                                                  std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                                                  std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
        : mInputLevelMeter (inputLevelMeter), mClippingLevelMeter (clippingLevelMeter), mOutputLevelMeter (outputLevelMeter), mInputPeakMeter (gAnalyzerSampleRateHz, gMinDbValInOut), mOutputPeakMeter (gAnalyzerSampleRateHz, gMinDbValInOut), mEatenPeakMeter (gAnalyzerSampleRateHz, gMinDbValEaten), mUpdateTimer (std::bind (&PeakAnalyzerComponent::onUpdateTick, this)), mInputLabel(), mOutputLabel(), mEatenLabel()
    {
        mInputLabel.setJustificationType (juce::Justification::verticallyCentred);
        mOutputLabel.setJustificationType (juce::Justification::verticallyCentred);
        mEatenLabel.setJustificationType (juce::Justification::verticallyCentred);

        addAndMakeVisible (mInputLabel);
        addAndMakeVisible (mOutputLabel);
        addAndMakeVisible (mEatenLabel);
        mUpdateTimer.startTimerHz (gAnalyzerSampleRateHz);

        setMouseCursor (juce::MouseCursor::PointingHandCursor);
        mInputLabel.setMouseCursor (juce::MouseCursor::PointingHandCursor);
        mOutputLabel.setMouseCursor (juce::MouseCursor::PointingHandCursor);
        mEatenLabel.setMouseCursor (juce::MouseCursor::PointingHandCursor);

        addMouseListener (this, true);
    }

    PeakAnalyzerComponent::~PeakAnalyzerComponent()
    {
        setLookAndFeel (nullptr);
    }

    void PeakAnalyzerComponent::resized()
    {
        juce::Grid grid;
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        grid.templateRows = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = { Item (mInputLabel), Item (mOutputLabel), Item (mEatenLabel) };

        auto const fontSize = calculatePrimaryTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
        mInputLabel.setFont (fontSize);
        mOutputLabel.setFont (fontSize);
        mEatenLabel.setFont (fontSize);

        grid.performLayout (getLocalBounds());
    }

    void PeakAnalyzerComponent::paint (juce::Graphics&)
    {
        auto const inputAmount = mInputPeakMeter.getMagnitude();
        auto const outputAmount = mOutputPeakMeter.getMagnitude();
        auto const eatenAmount = mEatenPeakMeter.getMagnitude();

        if (gIsInBounds (eatenAmount, 10.0f, 20.0f))
        {
            mEatenLabel.setColour (juce::Label::textColourId, colourscheme::Warning);
        }
        else if (gIsInBounds (eatenAmount, 20.0f, std::numeric_limits<float>::max()))
        {
            mEatenLabel.setColour (juce::Label::textColourId, colourscheme::ForegroundSecondary);
        }
        else
        {
            mEatenLabel.setColour (juce::Label::textColourId, colourscheme::TextFocusLevel0);
        }

        auto const stringifyMagnitude = [] (float magnitude)
        {
            return gToStringWithPrecision (gRoundDb (magnitude), 1) + " dB";
        };
        mInputLabel.setText ("Input: " + stringifyMagnitude (inputAmount), juce::NotificationType::dontSendNotification);
        mOutputLabel.setText ("Output: " + stringifyMagnitude (outputAmount), juce::NotificationType::dontSendNotification);
        mEatenLabel.setText ("Eaten: " + stringifyMagnitude (eatenAmount), juce::NotificationType::dontSendNotification);
    }

    void PeakAnalyzerComponent::mouseDown (juce::MouseEvent const& event)
    {
        if (event.getNumberOfClicks() == 2)
        {
            mInputPeakMeter.reset();
            mOutputPeakMeter.reset();
            mEatenPeakMeter.reset();
        }
    }

    void PeakAnalyzerComponent::onUpdateTick()
    {
        auto const inputLevel = static_cast<float> (mInputLevelMeter->getDecibels());
        auto const outputLevel = static_cast<float> (mOutputLevelMeter->getDecibels());
        auto const clippingLevel = static_cast<float> (mClippingLevelMeter->getDecibels());
        mInputPeakMeter.push (inputLevel);
        mOutputPeakMeter.push (outputLevel);
        mEatenPeakMeter.push (inputLevel - clippingLevel);
    }
} // namespace gui
} // namespace pe