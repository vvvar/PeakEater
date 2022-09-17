#include "LevelMeter.h"

namespace pe
{
namespace dsp
{
namespace
{
float constexpr gDefaultLevel = 0.0f;
float constexpr gMinusInfinity = -36.0f;

template <typename T>
T gMagnitudeToDecibels (T const& magnitude)
{
    // return 20.0f * std::log10 (magnitude);
    return juce::Decibels::gainToDecibels<T> (magnitude, gMinusInfinity);
}

template <typename T>
T gCalculateAmplification (int const& channelIndex, juce::AudioBuffer<T> const& buffer)
{
    // return buffer.getRMSLevel (channelIndex, 0, buffer.getNumSamples());
    return buffer.getMagnitude (0, buffer.getNumSamples());
}

template <typename T>
T gCalculateDecibels (juce::AudioBuffer<T> const& buffer)
{
    auto const magnitude = gCalculateAmplification (0, buffer);
    return gMagnitudeToDecibels (magnitude);
}
} // namespace

template <typename T>
LevelMeter<T>::LevelMeter()
    : mDecibels (gDefaultLevel)
    , mAmplificationL (gDefaultLevel)
    , mAmplificationR (gDefaultLevel)
{
}

template <typename T>
void LevelMeter<T>::updateLevels (juce::AudioBuffer<T> const& buffer)
{
    mDecibels = gCalculateDecibels (buffer);
    mAmplificationL = gCalculateAmplification (0, buffer);
    mAmplificationR = gCalculateAmplification (1, buffer);
}

template <typename T>
std::atomic<T>& LevelMeter<T>::getDecibels()
{
    return mDecibels;
}

template <typename T>
std::tuple<std::atomic<T>&, std::atomic<T>&> LevelMeter<T>::getAmplification()
{
    return std::forward_as_tuple (mAmplificationL, mAmplificationR);
}

template class LevelMeter<float>;
template class LevelMeter<double>;
} // namespace dsp
} // namespace pe