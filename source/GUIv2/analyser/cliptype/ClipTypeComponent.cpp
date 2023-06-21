#include "ClipTypeComponent.h"

#include <functional>

#include "../../../DSP/ClippingFunctions.h"
#include "../../ColourScheme.h"
#include "../../Utils.h"
#include <JuceHeader.h>

namespace pe
{
namespace gui
{
    namespace
    {
        int constexpr gBorderWidth = 1;
        int constexpr gBorderRadius = 10;
        float constexpr gWaveThickness = 1.0f;
        auto constexpr gParamName = "ClippingType";

        template <typename T>
        bool gIsInBounds (const T& value, const T& low, const T& high)
        {
            return ! (value < low) && (value < high);
        }

        std::vector<float> generateClippedSineWave (std::function<float (float)> clippingFunction)
        {
            float const frequency = 1.0f;
            float const amplitude = 1.0f;
            float const sampleRate = 77.0f;
            float const pi = juce::MathConstants<float>::pi;
            float const gain = 1.8f;

            std::vector<float> sineWave;
            for (int i = 0; i < static_cast<int> (sampleRate); i++)
            {
                float const sampleNumber = static_cast<float> (i);
                float const sample = amplitude * sinf (2.0f * pi * frequency * sampleNumber / sampleRate);
                float const clippedSample = clippingFunction (sample * gain);
                sineWave.push_back (clippedSample);
            }

            return sineWave;
        }
    } // namespace
    ClipTypeComponent::ClipTypeComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters)
        : mParameters (parameters)
    {
        mClippedWaveTables.emplace ("DEFAULT", std::vector<float>());
        mClippedWaveTables.emplace ("HARD", generateClippedSineWave (dsp::waveshaping::hardclip<float>));
        mClippedWaveTables.emplace ("QUINTIC", generateClippedSineWave (dsp::waveshaping::quintic<float>));
        mClippedWaveTables.emplace ("CUBIC", generateClippedSineWave (dsp::waveshaping::cubicBasic<float>));
        mClippedWaveTables.emplace ("TANGENT", generateClippedSineWave (dsp::waveshaping::tanclip<float>));
        mClippedWaveTables.emplace ("ALGEBRAIC", generateClippedSineWave (dsp::waveshaping::algClip<float>));
        mClippedWaveTables.emplace ("ARCTANGENT", generateClippedSineWave (dsp::waveshaping::arcClip<float>));
    }

    ClipTypeComponent::~ClipTypeComponent()
    {
        setLookAndFeel (nullptr);
    }

    void ClipTypeComponent::paint (juce::Graphics& g)
    {
        auto bounds = getLocalBounds().toFloat().reduced (gBorderWidth);

        juce::Path p;
        float offset = bounds.getX();
        auto const clippedWave = getCurrentWave();
        float const offsetStep = bounds.getRight() / clippedWave.size();
        p.startNewSubPath ({ bounds.getX(), bounds.getCentreY() });
        for (auto& sample : clippedWave)
        {
            p.lineTo ({ offset, bounds.getCentreY() + (sample * bounds.getHeight() / 3) });
            offset = offset + offsetStep;
        }
        g.setColour (colourscheme::ForegroundTertiary);
        g.strokePath (p, juce::PathStrokeType (gWaveThickness, juce::PathStrokeType::JointStyle::curved));

        g.setColour (colourscheme::BackgroundTertiary.withAlpha (0.5f));
        g.drawRoundedRectangle (bounds, gBorderRadius, gBorderWidth); // Draw border
    }

    void ClipTypeComponent::mouseDown (juce::MouseEvent const& event)
    {
        if (event.getNumberOfClicks() == 4) // Only for debug, to show current version
        {
            std::string const lModalBoxTitle = std::string (ProjectInfo::projectName) + " by " + std::string (ProjectInfo::companyName);
            std::string const lModalBoxText = "Version: " + std::string (ProjectInfo::versionString);
            juce::NativeMessageBox::showMessageBoxAsync (juce::AlertWindow::InfoIcon, lModalBoxTitle, lModalBoxText, nullptr, nullptr);
        }
    }

    std::vector<float> const& ClipTypeComponent::getCurrentWave() const
    {
        auto const clippingType = mParameters->getParameter (gParamName)->getCurrentValueAsText();
        if (mClippedWaveTables.find (clippingType) == mClippedWaveTables.end())
        {
            return mClippedWaveTables.at ("DEFAULT");
        }
        else
        {
            return mClippedWaveTables.at (clippingType);
        }
    }
} // namespace gui
} // namespace pe