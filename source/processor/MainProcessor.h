#pragma once

#include <juce_dsp/juce_dsp.h>

#include "processor/Clipper.h"

namespace pe::processor {

class MainProcessor {
   public:
    struct Parameters {
        float inputGain = 0.0f;
        size_t factorOversampling = 0;
        ClippingType clippingType = ClippingType::HARD;
        float ceiling = 0.0f;
        float outputGain = 0.0f;
        float dryWetProportion = 0.0f;
    };
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

    juce::dsp::Gain<float> inputGain;
    ClippingBlock<float> clippingBlock;
    juce::dsp::Gain<float> outputGain;
    EvenHandlers eventHandlers;
};

}  // namespace pe::processor
