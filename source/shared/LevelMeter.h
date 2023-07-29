#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

#include <atomic>
#include <tuple>

namespace pe::dsp {

template <typename T>
class LevelMeter {
   public:
    LevelMeter();

    void updateLevels(juce::dsp::AudioBlock<T> const& audioBlock);

    std::atomic<T>& getDecibels();
    std::tuple<std::atomic<T>&, std::atomic<T>&> getAmplification();

   private:
    std::atomic<T> mDecibels;
    std::atomic<T> mAmplificationL;
    std::atomic<T> mAmplificationR;
};

}  // namespace pe::dsp
