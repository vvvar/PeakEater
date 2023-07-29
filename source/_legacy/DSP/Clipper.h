#pragma once

#include <juce_dsp/juce_dsp.h>

#include "ClippingFunctions.h"

namespace pe::dsp::waveshaping {

// ==============================================================================
/* Available types of clipping */
enum ClippingType {
    LOGARYTHMIC = 0,
    HARD = 1,
    QUINTIC = 2,
    CUBIC = 3,
    HYPERBOLIC_TAN = 4,
    ALGEBRAIC = 5,
    ARCTANGENT = 6,
    SIN = 7,
    LIMIT = 8,
};

// ==============================================================================
template <typename T>
class Clipper : public juce::dsp::ProcessorBase {
   public:
    // ==============================================================================
    /* Iherited overriden methods */
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept override {
        preGain.prepare(spec);
        preGain.setRampDurationSeconds(DEFAULT_RAMP_DURATION);
        postGain.prepare(spec);
        postGain.setRampDurationSeconds(DEFAULT_RAMP_DURATION);
        setThreshold(DEFAULT_THRESHOLD);
        waveshaper.prepare(spec);
        setClippingType(DEFAULT_CLIPPING_TYPE);
    }

    void reset() noexcept override {
        preGain.reset();
        waveshaper.reset();
        postGain.reset();
    }

    void process(const juce::dsp::ProcessContextReplacing<T>& context) noexcept override {
        preGain.process(context);
        waveshaper.process(context);
        postGain.process(context);
    }

    // ==============================================================================
    /*
     * Change clipping function
     * that will be used to process signal
     */
    void setClippingType(ClippingType type) noexcept {
        switch (type) {
            case LOGARYTHMIC:
                waveshaper.functionToUse = logiclip;
                break;
            case HARD:
                waveshaper.functionToUse = hardclip;
                break;
            case QUINTIC:
                waveshaper.functionToUse = quintic;
                break;
            case CUBIC:
                waveshaper.functionToUse = cubicBasic;
                break;
            case HYPERBOLIC_TAN:
                waveshaper.functionToUse = tanclip;
                break;
            case ALGEBRAIC:
                waveshaper.functionToUse = algClip;
                break;
            case ARCTANGENT:
                waveshaper.functionToUse = arcClip;
                break;
            case SIN:
                waveshaper.functionToUse = sinclip;
                break;
            case LIMIT:
                waveshaper.functionToUse = limitclip;
                break;
            default:
                waveshaper.functionToUse = hardclip;
                break;
        }
    }

    /*
     * Set threshold(in decibels) when clipping will occur
     */
    void setThreshold(float threshold) noexcept {
        jassert(threshold <= 0.0f);  // threshold cannot be more than 0.0 dB

        preGain.setGainDecibels(std::fabs(threshold));  // gain signal up to make it clip
        postGain.setGainDecibels(threshold);            // gain down to make it's level
                                                        // tha same it was before clip
    }

   private:
    // ==============================================================================
    /** Consts */
    const float DEFAULT_THRESHOLD = 0.0f;
    const float DEFAULT_RAMP_DURATION = 0.1f;
    const ClippingType DEFAULT_CLIPPING_TYPE = ClippingType::HARD;

    // ==============================================================================
    /* DSP */
    juce::dsp::Gain<T> preGain;
    juce::dsp::Gain<T> postGain;
    juce::dsp::WaveShaper<T> waveshaper;
};
}  // namespace pe::dsp::waveshaping
