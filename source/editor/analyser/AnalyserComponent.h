#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../LevelMetersPack.h"
#include "./cliptype/ClipTypeComponent.h"
#include "./peakanalyzer/PeakAnalyzerComponent.h"

namespace pe
{
namespace gui
{
    class AnalyserComponent : public juce::Component
    {
    public:
        AnalyserComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                           LevelMetersPack const& levelMetersPack);
        ~AnalyserComponent() override;

        void resized() override;
        void paint (juce::Graphics& g) override;

    private:
        ClipTypeComponent mClipTypeComponent;
        PeakAnalyzerComponent mPeakAnalyzerComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalyserComponent)
    };
} // namespace gui
} // namespace pe