/*
  ==============================================================================

    ClippingFunctions.h
    Created: 3 Feb 2021 12:29:47pm
    Author:  Vladyslav Voinov

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include <math.h>
#include "Helpers.h"

namespace waveshaping
{

using JMath = juce::dsp::FastMathApproximations;

template<typename T>
T logiclip(T s) noexcept
{
    return 2.0f / (1.0f + JMath::exp(-2.0f * s)) - 1.0f;
}

template<typename T>
T hardclip(T s) noexcept
{
    return sgn(s) * std::fminf(std::fabsf(s), 1.0f);
}

template<typename T>
T tanclip(T s) noexcept
{
    float soft = 0.0f;
    return JMath::tanh((1.0f - 0.5f * soft) * s);
}

template<typename T>
T quintic(T s) noexcept
{
    if (std::fabsf(s) < 1.25f)
    {
        return s - (256.0f / 3125.0f) * std::powf(s, 5.0f);
    } else
    {
        return sgn(s) * 1.0f;
    }
}

template<typename T>
T cubicBasic(T s) noexcept
{
    if (std::fabsf(s) < 1.5f)
    {
        return s - (4.0f / 27.0f) * std::powf(s, 3.0f);
    } else
    {
        return sgn(s) * 1.0f;
    }
}

template<typename T>
T algclip(T s) noexcept
{
    float soft = 0.0f;
    return s / std::sqrtf((1.0f + 2.0f * soft + std::powf(s, 2.0f)));
}

template<typename T>
T arcClip(T s) noexcept
{
    float soft = 0.0f;
    return (2.0f / juce::MathConstants<T>::pi) * std::atanf((1.6f - soft * 0.6f) * s);
}

template<typename T>
T sinclip(T s) noexcept
{
    if (std::fabsf(s) < M_PI)
    {
        return JMath::sin(s);
    }
    else
    {
        return sgn(s) * 1.0f;
    }
}

template<typename T>
T limitclip(T x) noexcept
{
    return juce::jlimit(-0.1f, 0.1f, x);
}

}
