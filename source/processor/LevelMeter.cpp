#include "LevelMeter.h"

namespace pe::processor {

namespace {
float constexpr gMinusInfinity = -36.0f;
}  // namespace

template <typename T>
LevelMeter<T>::LevelMeter() : dbfs(gMinusInfinity) {}

template <typename T>
void LevelMeter<T>::updateLevels(juce::dsp::AudioBlock<T> const& audioBlock) {
    auto const range = audioBlock.findMinAndMax();
    auto const magnitude = juce::jmax(range.getStart(), -range.getStart(), range.getEnd(), -range.getEnd());
    dbfs = juce::Decibels::gainToDecibels<T>(magnitude, gMinusInfinity);
}

template <typename T>
std::atomic<T>& LevelMeter<T>::getDBFS() {
    return dbfs;
}

template class LevelMeter<float>;
template class LevelMeter<double>;
}  // namespace pe::processor
