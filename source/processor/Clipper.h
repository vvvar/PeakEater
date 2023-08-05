#pragma once

#include <juce_dsp/juce_dsp.h>

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
    explicit Clipper(size_t const oversampleFactorToUse);

    explicit Clipper();

    void prepare(juce::dsp::ProcessSpec const& spec);

    void reset();

    void process(juce::dsp::ProcessContextReplacing<SampleType> const& context);

    /*! \brief Set wave shaper threshold.
     *
     *  Everything above this level would be clipped by wave shaper.
     */
    void setThreshold(SampleType const threshold);

    /*! \brief Set clipping type.
     *
     *  This will set sigmoid function used by wave shaper.
     */
    void setClippingType(ClippingType const type);

    [[nodiscard]] size_t getOversamplingFactor() const;

    /*! \brief Get latency in samples */
    [[nodiscard]] int getLatency() const;

   private:
    size_t oversamplingFactor = 0;

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<SampleType>, juce::dsp::IIR::Coefficients<SampleType>> preFilter;
    juce::dsp::Gain<SampleType> preGain;
    juce::dsp::Oversampling<SampleType> oversampler;
    juce::dsp::WaveShaper<SampleType> waveShaper;
    juce::dsp::Gain<SampleType> postGain;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<SampleType>, juce::dsp::IIR::Coefficients<SampleType>> postFilter;
};
}  // namespace pe::processor
