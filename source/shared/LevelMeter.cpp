#include "LevelMeter.h"

namespace pe::dsp {
namespace {
float constexpr gMinusInfinity = -36.0f;

template <typename T>
T gMagnitudeToDecibels(T const& magnitude) {
    return juce::Decibels::gainToDecibels<T>(magnitude, gMinusInfinity);
}

template <typename T>
T gCalculateAmplification(juce::dsp::AudioBlock<T> const& audioBlock) {
    return audioBlock.findMinAndMax().getEnd();
}

template <typename T>
T gCalculateDecibels(juce::dsp::AudioBlock<T> const& audioBlock) {
    auto const magnitude = gCalculateAmplification(audioBlock);
    return gMagnitudeToDecibels(magnitude);
}
}  // namespace

template <typename T>
LevelMeter<T>::LevelMeter() : mDecibels(gMinusInfinity), mAmplificationL(gMinusInfinity), mAmplificationR(gMinusInfinity) {}

template <typename T>
void LevelMeter<T>::updateLevels(juce::dsp::AudioBlock<T> const& audioBlock) {
    mDecibels = gCalculateDecibels(audioBlock);
    mAmplificationL = gCalculateAmplification(audioBlock);
    mAmplificationR = gCalculateAmplification(audioBlock);
}

template <typename T>
std::atomic<T>& LevelMeter<T>::getDecibels() {
    return mDecibels;
}

template <typename T>
std::tuple<std::atomic<T>&, std::atomic<T>&> LevelMeter<T>::getAmplification() {
    return std::forward_as_tuple(mAmplificationL, mAmplificationR);
}

template class LevelMeter<float>;
template class LevelMeter<double>;
}  // namespace pe::dsp
