/*
  ==============================================================================

    OversampledClipper.h
    Created: 3 Feb 2021 1:03:59pm
    Author:  Vladyslav Voinov

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "Clipper.h"

// ==============================================================================
namespace waveshaping
{

// ==============================================================================
template<typename T>
class OversampledClipper : public juce::dsp::ProcessorBase
{
public:
    //==============================================================================
    /** Aliases */
    using Oversampling  = juce::dsp::Oversampling<T>;
    using ProcessorSpec = juce::dsp::ProcessSpec;
    
    //==============================================================================
    OversampledClipper (const unsigned int factor = 1) noexcept :
        oversampler (2, factor, Oversampling::filterHalfBandFIREquiripple, false)
    {}
    
    ~OversampledClipper()
    {}
    
    //==============================================================================
    /** Inheritet implemented methods */
    void prepare (const ProcessorSpec& spec) noexcept override
    {
        /** Setup oversampler */
        oversampler.reset();
        oversampler.initProcessing (spec.maximumBlockSize);
        
        const auto oversampledSpec = createOversampledSpec (spec);
        
        /** Setup pre-filter*/
        preFilter.prepare (oversampledSpec);
        preFilter.setType (juce::dsp::StateVariableTPTFilterType::lowpass);
        preFilter.setCutoffFrequency (calculateCutoff (oversampledSpec.sampleRate));
        preFilter.setResonance (calculateQ(DEFAULT_FILTER_Q_OCTAVES));
        
        /** Setup clipper*/
        clipper.prepare (oversampledSpec);
        setCeiling (DEFAULT_CEILING);
        setClippingType (DEFAULT_CLIPPING_TYPE);
        
        /** Setup post-filter*/
        postFilter.prepare (oversampledSpec);
        postFilter.setType (juce::dsp::StateVariableTPTFilterType::lowpass);
        postFilter.setCutoffFrequency (calculateCutoff (oversampledSpec.sampleRate));
        postFilter.setResonance (calculateQ(DEFAULT_FILTER_Q_OCTAVES));
    }
    
    void reset() noexcept override
    {
        oversampler.reset();
        preFilter.reset();
        clipper.reset();
        postFilter.reset();
    }
    
    void process (const juce::dsp::ProcessContextReplacing<T>& context) noexcept override
    {
        auto oversampledAudioBlock = oversampler.processSamplesUp (context.getInputBlock());
        auto oversampledContext = juce::dsp::ProcessContextReplacing<T> (oversampledAudioBlock);

        preFilter.process (oversampledContext);
        clipper.process (oversampledContext);
        postFilter.process (oversampledContext);
        
        oversampler.processSamplesDown (context.getOutputBlock());
    }
    
    // ==============================================================================
    /** Public interface */
    void setCeiling (float ceilingDbValue) noexcept
    {
        clipper.setThreshold (ceilingDbValue);
    }
    
    void setClippingType (ClippingType clippingType) noexcept
    {
        clipper.setClippingType (clippingType);
    }
    
private:
    //==============================================================================
    /** Default values */
    const float        DEFAULT_FILTER_Q_OCTAVES = 4.0f;
    const float        DEFAULT_CEILING = 0.0f;
    const ClippingType DEFAULT_CLIPPING_TYPE = ClippingType::LOGARYTHMIC;
    
    //==============================================================================
    /** DSP */
    Oversampling oversampler;
    Clipper<T> clipper;
    juce::dsp::StateVariableTPTFilter<T> preFilter;
    juce::dsp::StateVariableTPTFilter<T> postFilter;
    
    //==============================================================================
    /** Filter calculations based on input params */
    unsigned int calculateCutoff (double sampleRate) const noexcept
    {
        return (sampleRate / 2) * 0.98;
    }
    
    double calculateQ (float octaves = 4.0f) const noexcept
    {
        jassert (octaves >= 0.0f & octaves <= 4); // maximum 4 octaves allowed, no negative octaves
        
        return 1.0f / std::sqrtf (octaves);
    }
    
    //==============================================================================
    /** Helper builders */
    const ProcessorSpec createOversampledSpec (const ProcessorSpec& src) const noexcept
    {
        const unsigned int xOversample = std::sqrt (oversampler.getOversamplingFactor());
        return { src.sampleRate * xOversample, src.maximumBlockSize * xOversample, src.numChannels };
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OversampledClipper)
};

}

