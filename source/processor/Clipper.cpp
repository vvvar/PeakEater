#include "Clipper.h"

#include <cmath>

#include "Sigmoid.h"

namespace pe::processor {

namespace {
/*! \brief Calculate frequency of cutoff based on sample rate */
template <typename T>
[[nodiscard]] T calculateCutoff(double const sampleRate) {
    // Start cutting of from Nyquist with smoothing
    // so that we have a steep cutoff that ends at the end of the spectrum
    auto const nyquistFrequency = static_cast<T>(sampleRate) / static_cast<T>(2.0);
    auto const smoothing = static_cast<T>(0.98);
    auto const cutoff = static_cast<T>(nyquistFrequency * smoothing);
    return cutoff;
}

[[nodiscard]] juce::dsp::ProcessSpec createOversampledSpec(juce::dsp::ProcessSpec const& src, size_t const oversamplingFactor) {
    auto const xOversample = static_cast<unsigned int>(::pow(2, oversamplingFactor));
    auto const oversampledSampleRate = src.sampleRate * xOversample;
    auto const maximumBlockSize = src.maximumBlockSize * xOversample;
    return juce::dsp::ProcessSpec{
        .sampleRate = oversampledSampleRate, .maximumBlockSize = maximumBlockSize, .numChannels = src.numChannels};
}
}  // namespace

template <typename SampleType>
Clipper<SampleType>::Clipper(size_t const oversampleFactorToUse)
    : oversamplingFactor(oversampleFactorToUse),
      oversampler(2, oversampleFactorToUse, juce::dsp::Oversampling<SampleType>::FilterType::filterHalfBandFIREquiripple, false, true) {}

template <typename SampleType>
Clipper<SampleType>::Clipper() : Clipper(0) {}

template <typename SampleType>
void Clipper<SampleType>::prepare(juce::dsp::ProcessSpec const& spec) {
    //-----------------------------------------------------------
    // Setup oversampling and obtain oversampled context to configure the rest of the chain
    oversampler.reset();
    oversampler.numChannels = spec.numChannels;
    oversampler.initProcessing(spec.maximumBlockSize);
    const auto oversampledSpec = createOversampledSpec(spec, oversamplingFactor);
    //-----------------------------------------------------------
    // Setup pre-clipping filter - add proper filter and prepare
    *preFilter.state = *juce::dsp::IIR::Coefficients<SampleType>::makeLowPass(oversampledSpec.sampleRate,
                                                                              calculateCutoff<SampleType>(oversampledSpec.sampleRate));
    preFilter.prepare(oversampledSpec);
    //-----------------------------------------------------------
    // Setup pre-gain
    preGain.prepare(oversampledSpec);
    preGain.setRampDurationSeconds(0.1f);
    //-----------------------------------------------------------
    // Prepare wave shaper
    waveShaper.prepare(oversampledSpec);
    //-----------------------------------------------------------
    // Prepare post-gain
    postGain.prepare(oversampledSpec);
    postGain.setRampDurationSeconds(0.1f);
    //-----------------------------------------------------------
    // Prepare post-filter
    *postFilter.state = *juce::dsp::IIR::Coefficients<SampleType>::makeLowPass(oversampledSpec.sampleRate,
                                                                               calculateCutoff<SampleType>(oversampledSpec.sampleRate));
    postFilter.prepare(oversampledSpec);
}

template <typename SampleType>
void Clipper<SampleType>::reset() {
    oversampler.reset();
    preFilter.reset();
    preGain.reset();
    waveShaper.reset();
    postGain.reset();
    postFilter.reset();
}

template <typename SampleType>
void Clipper<SampleType>::process(juce::dsp::ProcessContextReplacing<SampleType> const& context) {
    //-----------------------------------------------------------
    // Create oversampled context
    // NOTE: When oversampling factor is 0 this will return normal audio block
    auto oversampledAudioBlock = oversampler.processSamplesUp(context.getInputBlock());
    auto oversampledContext = juce::dsp::ProcessContextReplacing<SampleType>(oversampledAudioBlock);
    //-----------------------------------------------------------
    // Process the signal using oversampled context
    //-----------------------------------------------------------
    // First, filter everything above Nyquist freq. Otherwise,
    // running these freq's through wave shaping function will
    // produce aliasing(even on oversampled signal)
    // NOTE: Relevant only for oversampled clipping since
    //       applying this to the non-oversampled signal will
    //       cut frequencies within the hearing spectrum
    if (oversamplingFactor > 0) {
        preFilter.process(oversampledContext);
    }
    //-----------------------------------------------------------
    // Then, boost gain so that signal will activate sigmoid
    // function in wave shaper(and limit it)
    preGain.process(oversampledContext);
    //-----------------------------------------------------------
    // Finally, we can pass samples to the sigmoid and limit them
    waveShaper.process(oversampledContext);
    //-----------------------------------------------------------
    // Bring original gain back
    postGain.process(oversampledContext);
    //-----------------------------------------------------------
    // Filter anything above Nyquist till the end of the spectrum
    // so that we will not abruptly cut them while down sampling
    // NOTE: Relevant only for oversampled clipping since
    //       applying this to the non-oversampled signal will
    //       cut frequencies within the hearing spectrum
    if (oversamplingFactor > 0) {
        postFilter.process(oversampledContext);
    }
    //-----------------------------------------------------------
    // Finally, down sample everything to the original sample rate
    oversampler.processSamplesDown(context.getOutputBlock());
}

template <typename SampleType>
void Clipper<SampleType>::setThreshold(SampleType const threshold) {
    // Sigmoid function activates at 0.0db
    // so that threshold cannot be more than that
    jassert(threshold <= 0.0f);
    // Gain signal up so sigmoid function can limit it.
    // We will do it before applying a sigmoid function
    preGain.setGainDecibels(std::fabs(threshold));
    // Gain down to make it's level tha same it was before clip
    // We will do it after we have applied a sigmoid function
    postGain.setGainDecibels(threshold);
}

template <typename SampleType>
void Clipper<SampleType>::setClippingType(ClippingType const type) {
    switch (type) {
        case ClippingType::LOGARITHMIC:
            waveShaper.functionToUse = processor::logiclip;
            break;
        case ClippingType::HARD:
            waveShaper.functionToUse = processor::hardclip;
            break;
        case ClippingType::QUINTIC:
            waveShaper.functionToUse = processor::quintic;
            break;
        case ClippingType::CUBIC:
            waveShaper.functionToUse = processor::cubicBasic;
            break;
        case ClippingType::HYPERBOLIC_TAN:
            waveShaper.functionToUse = processor::tanclip;
            break;
        case ClippingType::ALGEBRAIC:
            waveShaper.functionToUse = processor::algClip;
            break;
        case ClippingType::ARCTANGENT:
            waveShaper.functionToUse = processor::arcClip;
            break;
        case ClippingType::SIN:
            waveShaper.functionToUse = processor::sinclip;
            break;
        case ClippingType::LIMIT:
            waveShaper.functionToUse = processor::limitclip;
            break;
        default:
            waveShaper.functionToUse = processor::hardclip;
            break;
    }
}

template <typename SampleType>
[[nodiscard]] size_t Clipper<SampleType>::getOversamplingFactor() const {
    return oversamplingFactor;
}

template <typename SampleType>
[[nodiscard]] SampleType Clipper<SampleType>::getLatency() const {
    return oversampler.getLatencyInSamples();
}

}  // namespace pe::processor

//-----------------------------------------------------------
// Explicit template instantiation because c++ ¯\_(ツ)_/¯
template class pe::processor::Clipper<float>;
