#include "MainProcessor.h"

namespace pe::processor {

void MainProcessor::prepare(juce::dsp::ProcessSpec const& spec, EvenHandlers const& handlers) {
    inputGain.prepare(spec);
    clippingBlock.prepare(spec);
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
    clippingBlock.process(context);
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
    outputGain.reset();
    clippingBlock.reset();
    inputGain.reset();
}

void MainProcessor::updateParameters(Parameters const&& parameters) {
    inputGain.setGainDecibels(parameters.inputGain);
    clippingBlock.setOversamplingFactor(parameters.factorOversampling);
    clippingBlock.setDryWetProportion(parameters.dryWetProportion);
    clippingBlock.setClippingType(parameters.clippingType);
    clippingBlock.setThreshold(parameters.ceiling);
    outputGain.setGainDecibels(parameters.outputGain);
}

}  // namespace pe::processor
