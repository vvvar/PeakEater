#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "PluginProcessor.h"
#include "editor/LevelMetersPack.h"
#include "editor/MainComponent.h"

namespace pe {
class PeakEaterAudioProcessorEditor : public juce::AudioProcessorEditor {
   public:
    PeakEaterAudioProcessorEditor(PeakEaterAudioProcessor& audioProcessor, std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                                  gui::LevelMetersPack const&& levelMetersPack);
    ~PeakEaterAudioProcessorEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

   private:
    pe::gui::MainComponent mMainComponent;
    PeakEaterAudioProcessor& mAudioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PeakEaterAudioProcessorEditor)
};
}  // namespace pe
