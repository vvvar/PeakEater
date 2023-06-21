#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../DSP/LevelMeter.h"
#include "Ticks.h"
#include "levelmeter/LevelMeterComponent.h"

namespace pe
{
namespace gui
{

    class RightPanel : public juce::Component
    {
    public:
        RightPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                    std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                    std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                    std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter,
                    std::shared_ptr<Ticks> ticks);

        void resized() override;

    private:
        LevelMeterComponent mLevelMeter;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightPanel)
    };
} // namespace gui
} // namespace pe