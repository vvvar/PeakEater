#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

#include <atomic>
#include <tuple>

namespace pe::processor {

template <typename T>
class LevelMeter {
   public:
    LevelMeter();

    void updateLevels(juce::dsp::AudioBlock<T> const& audioBlock);

    std::atomic<T>& getDBFS();

   private:
    std::atomic<T> dbfs;
};

}  // namespace pe::processor
