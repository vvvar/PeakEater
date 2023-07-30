#pragma once

#include <juce_dsp/juce_dsp.h>

#include <cmath>

#include "shared/ClippingFunctions.h"

namespace pe::processor {

enum class ClippingType {
    LOGARITHMIC = 0,
    HARD = 1,
    QUINTIC = 2,
    CUBIC = 3,
    HYPERBOLIC_TAN = 4,
    ALGEBRAIC = 5,
    ARCTANGENT = 6,
    SIN = 7,
    LIMIT = 8,
};

template <typename SampleType>
class Clipper {
   public:
    explicit Clipper(size_t const oversampleFactorToUse)
        : oversamplingFactor(oversampleFactorToUse),
          oversampler(2, oversampleFactorToUse, juce::dsp::Oversampling<SampleType>::FilterType::filterHalfBandFIREquiripple) {}

    explicit Clipper() : Clipper(0) {}

    void prepare(juce::dsp::ProcessSpec const& spec) {
        //-----------------------------------------------------------
        // Setup oversampling and obtain oversampled context to configure the rest of the chain
        oversampler.reset();
        oversampler.numChannels = spec.numChannels;
        oversampler.initProcessing(spec.maximumBlockSize);
        const auto oversampledSpec = createOversampledSpec(spec);
        //-----------------------------------------------------------
        // Setup pre-clipping filter - add proper filter and prepare
        *preFilter.state =
            *juce::dsp::IIR::Coefficients<SampleType>::makeLowPass(oversampledSpec.sampleRate, calculateCutoff(oversampledSpec.sampleRate));
        preFilter.prepare(oversampledSpec);
        //-----------------------------------------------------------
        // Setup pre-gain
        preGain.prepare(oversampledSpec);
        preGain.setRampDurationSeconds(0.1f);
        //-----------------------------------------------------------
        // Setup dry/wet
        dryWet.prepare(oversampledSpec);
        dryWet.setMixingRule(juce::dsp::DryWetMixingRule::balanced);
        //-----------------------------------------------------------
        // Prepare wave shaper
        waveShaper.prepare(oversampledSpec);
        //-----------------------------------------------------------
        // Prepare post-gain
        postGain.prepare(oversampledSpec);
        postGain.setRampDurationSeconds(0.1f);
        //-----------------------------------------------------------
        // Prepare post-filter
        *postFilter.state =
            *juce::dsp::IIR::Coefficients<SampleType>::makeLowPass(oversampledSpec.sampleRate, calculateCutoff(oversampledSpec.sampleRate));
        postFilter.prepare(oversampledSpec);
    }

    void reset() noexcept {
        oversampler.reset();
        preFilter.reset();
        preGain.reset();
        dryWet.reset();
        waveShaper.reset();
        postGain.reset();
        postFilter.reset();
    }

    void process(juce::dsp::ProcessContextReplacing<SampleType> const& context) {
        //-----------------------------------------------------------
        // Create oversampled context
        auto oversampledAudioBlock = oversampler.processSamplesUp(context.getInputBlock());
        auto oversampledContext = juce::dsp::ProcessContextReplacing<SampleType>(oversampledAudioBlock);
        //-----------------------------------------------------------
        // Process the signal using oversampled context
        //-----------------------------------------------------------
        // First, filter out everything beyond Nyquist because these
        // freq's are beyond human hearing anyways and we are not
        // interested in processing them
        preFilter.process(oversampledContext);
        //-----------------------------------------------------------
        // Then, boost gain so that signal will activate sigmoid
        // function in wave shaper which will limit(clip) the signal
        preGain.process(oversampledContext);
        //-----------------------------------------------------------
        // Before we clipped the signal - remember the dry signal.
        // We are doing this exactly before and right after the
        // wave shaping to avoid comb filter effect
        dryWet.pushDrySamples(oversampledContext.getInputBlock());
        //-----------------------------------------------------------
        // Finally, we can pass samples to the sigmoid and limit them
        waveShaper.process(oversampledContext);
        //-----------------------------------------------------------
        // Immediately mix them with dry signal to avoid any comb
        dryWet.mixWetSamples(oversampledContext.getOutputBlock());
        //-----------------------------------------------------------
        // Bring original gain back
        postGain.process(oversampledContext);
        //-----------------------------------------------------------
        // Filter anything beyond Nyquist again so that we will not
        // abruptly cut them while down sampling
        postFilter.process(oversampledContext);
        //-----------------------------------------------------------
        // Finally, down sample everything back
        oversampler.processSamplesDown(context.getOutputBlock());
    }

    /*! \brief Set wave shaper threshold.
     *
     *  Everything above this level would be clipped by wave shaper.
     */
    void setThreshold(SampleType const threshold) {
        // threshold cannot be more than 0.0 dB
        jassert(threshold <= 0.0f);
        // gain signal up to make it clip
        preGain.setGainDecibels(std::fabs(threshold));
        // gain down to make it's level tha same it was before clip
        postGain.setGainDecibels(threshold);
    }

    /*! \brief Set clipping type.
     *
     *  This will set sigmoid function used by wave shaper.
     */
    void setClippingType(ClippingType const type) {
        switch (type) {
            case ClippingType::LOGARITHMIC:
                waveShaper.functionToUse = shared::logiclip;
                break;
            case ClippingType::HARD:
                waveShaper.functionToUse = shared::hardclip;
                break;
            case ClippingType::QUINTIC:
                waveShaper.functionToUse = shared::quintic;
                break;
            case ClippingType::CUBIC:
                waveShaper.functionToUse = shared::cubicBasic;
                break;
            case ClippingType::HYPERBOLIC_TAN:
                waveShaper.functionToUse = shared::tanclip;
                break;
            case ClippingType::ALGEBRAIC:
                waveShaper.functionToUse = shared::algClip;
                break;
            case ClippingType::ARCTANGENT:
                waveShaper.functionToUse = shared::arcClip;
                break;
            case ClippingType::SIN:
                waveShaper.functionToUse = shared::sinclip;
                break;
            case ClippingType::LIMIT:
                waveShaper.functionToUse = shared::limitclip;
                break;
            default:
                waveShaper.functionToUse = shared::hardclip;
                break;
        }
    }

    /*! \brief Set dry/wet mixing proportion.
     *
     * Signal would be mixed right after the wave shaping.
     */
    void setDryWetProportion(SampleType const proportion) {
        // 0.0 - fully dry(un-clipped)
        // 1.0 - fully wet(clipped)
        // Everything above/beyond - incorrect
        jassert(proportion >= static_cast<SampleType>(0.0) && proportion <= static_cast<SampleType>(1.0));
        dryWet.setWetMixProportion(proportion);
    }

    [[nodiscard]] size_t getOversamplingFactor() const { return oversamplingFactor; }

   private:
    size_t oversamplingFactor = 0;
    juce::dsp::Oversampling<SampleType> oversampler;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<SampleType>, juce::dsp::IIR::Coefficients<SampleType>> preFilter;
    juce::dsp::Gain<SampleType> preGain;
    juce::dsp::DryWetMixer<SampleType> dryWet;
    juce::dsp::WaveShaper<SampleType> waveShaper;
    juce::dsp::Gain<SampleType> postGain;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<SampleType>, juce::dsp::IIR::Coefficients<SampleType>> postFilter;

    //==============================================================================
    /*! \brief Filter calculations based on input params. */
    [[nodiscard]] unsigned int calculateCutoff(double const sampleRate) const {
        return static_cast<unsigned int>((sampleRate / 2.0) * 0.98);
    }

    SampleType calculateQ(size_t const octaves) const noexcept {
        // Maximum 4 octaves allowed, no negative octaves
        jassert(octaves >= static_cast<SampleType>(0.0) && octaves <= static_cast<SampleType>(4.0));
        return static_cast<SampleType>(1) / ::sqrt(static_cast<SampleType>(octaves));
    }

    //==============================================================================
    /*! \brief Create custom ProcessSpec since this processor could work with different in other sample rate. */
    [[nodiscard]] juce::dsp::ProcessSpec createOversampledSpec(juce::dsp::ProcessSpec const& src) const {
        auto const xOversample = static_cast<unsigned int>(::pow(2, oversamplingFactor));
        auto const oversampledSampleRate = src.sampleRate * xOversample;
        auto const maximumBlockSize = src.maximumBlockSize * xOversample;
        return juce::dsp::ProcessSpec{
            .sampleRate = oversampledSampleRate, .maximumBlockSize = maximumBlockSize, .numChannels = src.numChannels};
    }
};
}  // namespace pe::processor
