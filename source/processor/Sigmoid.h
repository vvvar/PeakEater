#pragma once

#include <juce_dsp/juce_dsp.h>

#include <cmath>

namespace pe::processor {

namespace {
template <typename T>
int sgn(T x) {
    return (T(0) < x) - (x < T(0));
}
}  // namespace

using JMath = juce::dsp::FastMathApproximations;

template <typename T>
T logiclip(T x) noexcept {
    return 2.0f / (1.0f + JMath::exp(-2.0f * x)) - 1.0f;
}

template <typename T>
T hardclip(T x) noexcept {
    return sgn(x) * fminf(fabsf(x), 1.0f);
}

template <typename T>
T tanclip(T x) noexcept {
    float soft = 0.0f;
    return std::tanh((1.0f - 0.5f * soft) * x);
}

template <typename T>
T quintic(T x) noexcept {
    if (fabsf(x) < 1.25f) {
        return x - (256.0f / 3125.0f) * powf(x, 5.0f);
    } else {
        return sgn(x) * 1.0f;
    }
}

template <typename T>
T cubicBasic(T x) noexcept {
    if (fabsf(x) < 1.5f) {
        return x - (4.0f / 27.0f) * powf(x, 3.0f);
    } else {
        return sgn(x) * 1.0f;
    }
}

template <typename T>
T algClip(T x) noexcept {
    float soft = 0.0f;
    return x / sqrtf((1.0f + 2.0f * soft + powf(x, 2.0f)));
}

template <typename T>
T arcClip(T x) noexcept {
    float soft = 0.0f;
    return (2.0f / juce::MathConstants<T>::pi) * atanf((1.6f - soft * 0.6f) * x);
}

template <typename T>
T sinclip(T x) noexcept {
    if (fabsf(x) < juce::MathConstants<T>::pi) {
        return JMath::sin(x);
    } else {
        return sgn(x) * 1.0f;
    }
}

template <typename T>
T limitclip(T x) noexcept {
    return juce::jlimit(-0.1f, 0.1f, x);
}
}  // namespace pe::processor
