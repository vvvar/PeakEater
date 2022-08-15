#pragma once

// #include <juce_events/juce_events.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
// #include <sound_meter/sound_meter.h>

#include "../DSP/LevelMeter.h"
#include "clipmeter/ClipMeter.h"

namespace pe
{
namespace gui
{

class MainComponent : public juce::Component
{
public:
    MainComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                   std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                   std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                   std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter);

    void resized() override;

private:
    ClipMeter mClipMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
} // namespace gui
} // namespace pe