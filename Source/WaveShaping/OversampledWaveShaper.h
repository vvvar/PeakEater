/*
  ==============================================================================

    OversampledWaveShaper.h
    Created: 3 Feb 2021 1:16:42pm
    Author:  Vladyslav Voinov

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "OversampledClipper.h"

namespace waveshaping
{

//==============================================================================
enum OversamplingRate
{
    X2 = 0,
    X4 = 1,
    X8 = 2,
    X16 = 3
};

// ==============================================================================
class OversampledWaveShaperListener
{
public:
    virtual ~OversampledWaveShaperListener() = default;
    
    virtual void onPostInputGain(juce::AudioBuffer<float>&) = 0;
    virtual void onPostCeiling(juce::AudioBuffer<float>&) = 0;
    virtual void onPostOutputGain(juce::AudioBuffer<float>&) = 0;
};

// ==============================================================================
template<typename T>
class OversampledWaveShaper : public juce::dsp::ProcessorBase
{
public:
    //==============================================================================
    /** Aliases */
    using ProcessContextReplacing = juce::dsp::ProcessContextReplacing<T>;
    using ProcessorSpec           = juce::dsp::ProcessSpec;
    using AudioBuffer             = juce::AudioBuffer<T>;
    using AudioBlock              = juce::dsp::AudioBlock<T>;
    
    //==============================================================================
    OversampledWaveShaper() noexcept :
        clipper2x (1),
        clipper4x (2),
        clipper8x (3),
        clipper16x (4),
        clipper (&clipper2x)
    {}
    
    ~OversampledWaveShaper()
    {}
    
    //==============================================================================
    /** Inheritet implemented methods */
    void prepare (const ProcessorSpec& spec) noexcept override
    {
        /** Setup input gain */
        inputGain.prepare (spec);
        inputGain.setRampDurationSeconds(RAMP_DURATION_SEC);
        setInputGain (DEFAULT_INPUT_GAIN);
        
        /** Setup clipper */
        clipper2x.prepare (spec);
        clipper4x.prepare (spec);
        clipper8x.prepare (spec);
        clipper16x.prepare (spec);
        
        /** Setup output gain */
        outputGain.prepare (spec);
        outputGain.setRampDurationSeconds(RAMP_DURATION_SEC);
        setOutputGain (DEFAULT_OUTPUT_GAIN);
    }
    
    void reset() noexcept override
    {
        inputGain.reset();
        clipper2x.reset();
        clipper4x.reset();
        clipper8x.reset();
        clipper16x.reset();
        outputGain.reset();
    }
    
    void process (const ProcessContextReplacing& context) noexcept override
    {
        inputGain.process (context);
        clipper->process (context);
        outputGain.process (context);
    }
    
    void process (AudioBuffer& buffer) noexcept
    {
        auto ioBlock = AudioBlock (buffer);
        ProcessContextReplacing context(ioBlock);
        
        inputGain.process (context);
        listenersList.call ([&buffer] (OversampledWaveShaperListener& listener)
        {
            listener.onPostInputGain (buffer);
        });
        clipper->process (context);
        listenersList.call ([&buffer] (OversampledWaveShaperListener& listener) {
            listener.onPostCeiling (buffer);
        });
        outputGain.process (context);
        listenersList.call ([&buffer] (OversampledWaveShaperListener& listener) {
            listener.onPostOutputGain (buffer);
        });
    }
    
    // ==============================================================================
    /** Public interface */
    void addListener (OversampledWaveShaperListener* listener) noexcept
    {
        listenersList.add(listener);
    }
    
    void setInputGain (float gainDbValue) noexcept
    {
        inputGain.setGainDecibels (gainDbValue);
    }
    
    void setOutputGain (float gainDbValue) noexcept
    {
        outputGain.setGainDecibels (gainDbValue);
    }
    
    void setCeiling (float ceilingDbValue) noexcept
    {
        clipper2x.setCeiling (ceilingDbValue);
        clipper4x.setCeiling (ceilingDbValue);
        clipper8x.setCeiling (ceilingDbValue);
        clipper16x.setCeiling (ceilingDbValue);
    }
    
    void setClippingType (ClippingType clippingType) noexcept
    {
        clipper2x.setClippingType (clippingType);
        clipper4x.setClippingType (clippingType);
        clipper8x.setClippingType (clippingType);
        clipper16x.setClippingType (clippingType);
    }
    
    void setOversamplingRate (OversamplingRate oversamplingRate) noexcept
    {
        switch (oversamplingRate)
        {
            case X2:
                clipper = &clipper2x;
                break;
            case X4:
                clipper = &clipper4x;
                break;
            case X8:
                clipper = &clipper8x;
                break;
            case X16:
                clipper = &clipper16x;
                break;
            default:
                clipper = &clipper2x;
                break;
        }
    }
private:
    //==============================================================================
    /** Default values */
    const float DEFAULT_INPUT_GAIN  = 0.0f;
    const float DEFAULT_OUTPUT_GAIN = 0.0f;
    const float RAMP_DURATION_SEC   = 0.1f;
    
    //==============================================================================
    /** DSP */
    juce::dsp::Gain<T>     inputGain;
    juce::dsp::Gain<T>     outputGain;
    OversampledClipper<T>  clipper2x;
    OversampledClipper<T>  clipper4x;
    OversampledClipper<T>  clipper8x;
    OversampledClipper<T>  clipper16x;
    OversampledClipper<T>* clipper;
    
    //==============================================================================
    /* Listeners */
    juce::ListenerList<OversampledWaveShaperListener> listenersList;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OversampledWaveShaper)
};

}
