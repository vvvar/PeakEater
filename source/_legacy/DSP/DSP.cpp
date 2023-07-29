#include "DSP.h"

namespace pe::dsp {

void MainProcessor::prepare(juce::dsp::ProcessSpec const& spec, EvenHandlers const& handlers) {
    dryWet.prepare(spec);
    inputGain.prepare(spec);
    // oversampler.prepare(spec);
    clipper.prepare(spec);
    outputGain.prepare(spec);
    eventHandlers = handlers;
}

void MainProcessor::process(juce::dsp::ProcessContextReplacing<float> const& context) {
    //-----------------------------------------------------------
    // Add input gain & report level before any processing
    inputGain.process(context);
    if (eventHandlers.postInput) {
        eventHandlers.postInput(context.getOutputBlock());
    }
    //-----------------------------------------------------------
    // Clip & report post-clip levels
    clipper.process(context);
    if (eventHandlers.postClipping) {
        eventHandlers.postClipping(context.getOutputBlock());
    }
    // Add output gain & report final output level
    outputGain.process(context);
    if (eventHandlers.postOutput) {
        eventHandlers.postOutput(context.getOutputBlock());
    }
}

void MainProcessor::reset() {
    // Unsubscribe handlers
    eventHandlers.postInput = nullptr;
    eventHandlers.postClipping = nullptr;
    eventHandlers.postOutput = nullptr;
    // Reset DSP
    dryWet.reset();
    outputGain.reset();
    clipper.reset();
    inputGain.reset();
}

void MainProcessor::updateParameters(Parameters const&& parameters) {
    if (inputGain.getGainDecibels() != parameters.inputGain) {
        inputGain.setGainDecibels(parameters.inputGain);
    }
    // if (oversampler.factor != parameters.factorOversampling) {
    //     oversampler.factor = parameters.factorOversampling;
    // }
    clipper.setClippingType(parameters.clippingType);
    clipper.setCeiling(parameters.ceiling);
    outputGain.setGainDecibels(parameters.outputGain);
    dryWet.setWetMixProportion(parameters.dryWetProportion);
}

}  // namespace pe::dsp
