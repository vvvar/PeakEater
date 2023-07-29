#pragma once

#include <juce_dsp/juce_dsp.h>

#include "Clipper.h"
#include "OversampledClipper.h"

namespace pe::dsp {

class MainProcessor {
   private:
    struct Parameters {
        float inputGain = 0.0f;
        int factorOversampling = 0;
        waveshaping::ClippingType clippingType = waveshaping::ClippingType::HARD;
        float ceiling = 0.0f;
        float outputGain = 0.0f;
        float dryWetProportion = 0.0f;
    };

   public:
    struct EvenHandlers {
        using Handler = std::function<void(juce::dsp::AudioBlock<float> const&)>;

        Handler postInput;
        Handler postClipping;
        Handler postOutput;
    };

    void prepare(juce::dsp::ProcessSpec const& spec, EvenHandlers const& handlers);

    void process(juce::dsp::ProcessContextReplacing<float> const& context);

    void reset();

    void updateParameters(Parameters const&& parameters);

   private:
    juce::dsp::DryWetMixer<float> dryWet;
    juce::dsp::Gain<float> inputGain;
    waveshaping::OversampledClipper<float> clipper{5};
    juce::dsp::Gain<float> outputGain;
    EvenHandlers eventHandlers;
};

}  // namespace pe::dsp
