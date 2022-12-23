/*
   ==============================================================================

    ClippingFunctions.h
    Created: 3 Feb 2021 12:29:47pm
    Author:  Vladyslav Voinov

   ==============================================================================
 */
#pragma once

#include <juce_dsp/juce_dsp.h>
#include <math.h>

#include "Helpers.h"

namespace pe
{
namespace dsp
{
namespace waveshaping
{

using JMath = juce::dsp::FastMathApproximations;

template <typename T>
T logiclip (T x) noexcept
{
	return 2.0f / (1.0f + JMath::exp (-2.0f * x)) - 1.0f;
}

template <typename T>
T hardclip (T x) noexcept
{
	return sgn (x) * std::fminf (std::fabsf (x), 1.0f);
}

template <typename T>
T tanclip (T x) noexcept
{
	float soft = 0.0f;
	return JMath::tanh ((1.0f - 0.5f * soft) * x);
}

template <typename T>
T quintic (T x) noexcept
{
	if (std::fabsf (x) < 1.25f)
	{
		return x - (256.0f / 3125.0f) * std::powf (x, 5.0f);
	}
	else
	{
		return sgn (x) * 1.0f;
	}
}

template <typename T>
T cubicBasic (T x) noexcept
{
	if (std::fabsf (x) < 1.5f)
	{
		return x - (4.0f / 27.0f) * std::powf (x, 3.0f);
	}
	else
	{
		return sgn (x) * 1.0f;
	}
}

template <typename T>
T algClip (T x) noexcept
{
	float soft = 0.0f;
	return x / std::sqrtf ((1.0f + 2.0f * soft + std::powf (x, 2.0f)));
}

template <typename T>
T arcClip (T x) noexcept
{
	float soft = 0.0f;
	return (2.0f / juce::MathConstants<T>::pi) * std::atanf ((1.6f - soft * 0.6f) * x);
}

template <typename T>
T sinclip (T x) noexcept
{
	if (std::fabsf (x) < juce::MathConstants<T>::pi)
	{
		return JMath::sin (x);
	}
	else
	{
		return sgn (x) * 1.0f;
	}
}

template <typename T>
T limitclip (T x) noexcept
{
	return juce::jlimit (-0.1f, 0.1f, x);
}
} // namespace waveshaping
} // namespace dsp
} // namespace pe
