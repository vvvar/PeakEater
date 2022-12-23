#pragma once

#include <atomic>
#include <juce_audio_basics/juce_audio_basics.h>
#include <tuple>

namespace pe
{
namespace dsp
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
} // namespace dsp
} // namespace pe