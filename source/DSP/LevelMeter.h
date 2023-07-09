#ifndef LEVELMETER_H
#define LEVELMETER_H

#include <atomic>
#include <juce_audio_basics/juce_audio_basics.h>
#include <tuple>

namespace pe::dsp
{

template <typename T>
class LevelMeter
{
public:
    LevelMeter();

    void updateLevels (juce::AudioBuffer<T> const& buffer);

    std::atomic<T>& getDecibels();
    std::tuple<std::atomic<T>&, std::atomic<T>&> getAmplification();

private:
    std::atomic<T> mDecibels;
    std::atomic<T> mAmplificationL;
    std::atomic<T> mAmplificationR;
};

} // namespace pe::dsp

#endif // COLOURSCHEME_H
