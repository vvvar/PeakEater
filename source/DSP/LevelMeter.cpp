#include "LevelMeter.h"

namespace pe
{
namespace dsp
{
    namespace
    {
        float constexpr gMinusInfinity = -36.0f;

        template <typename T>
        T gMagnitudeToDecibels (T const& magnitude)
        {
            return juce::Decibels::gainToDecibels<T> (magnitude, gMinusInfinity);
        }

        template <typename T>
        T gCalculateAmplification (juce::AudioBuffer<T> const& buffer)
        {
            return buffer.getMagnitude (0, buffer.getNumSamples());
        }

        template <typename T>
        T gCalculateDecibels (juce::AudioBuffer<T> const& buffer)
        {
            auto const magnitude = gCalculateAmplification (buffer);
            return gMagnitudeToDecibels (magnitude);
        }
    } // namespace

    template <typename T>
    LevelMeter<T>::LevelMeter()
        : mDecibels (gMinusInfinity), mAmplificationL (gMinusInfinity), mAmplificationR (gMinusInfinity)
    {
    }

    template <typename T>
    void LevelMeter<T>::updateLevels (juce::AudioBuffer<T> const& buffer)
    {
        mDecibels = gCalculateDecibels (buffer);
        mAmplificationL = gCalculateAmplification (buffer);
        mAmplificationR = gCalculateAmplification (buffer);
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