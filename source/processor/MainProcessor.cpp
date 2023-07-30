#include "MainProcessor.h"

namespace pe::processor {

void MainProcessor::prepare(juce::dsp::ProcessSpec const& spec, EvenHandlers const& handlers) {
    inputGain.prepare(spec);
    for (auto& clipper : clippers) {
        clipper.prepare(spec);
    }
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
    for (auto& clipper : clippers) {
        if (clipper.getOversamplingFactor() == oversamplingFactor) {
            clipper.process(context);
        }
    }
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
    // Reset DSP
    outputGain.reset();
    for (auto& clipper : clippers) {
        clipper.reset();
    }
    inputGain.reset();
    // Unsubscribe handlers
    eventHandlers.postInput = nullptr;
    eventHandlers.postClipping = nullptr;
    eventHandlers.postOutput = nullptr;
}

void MainProcessor::updateParameters(Parameters const&& parameters) {
    inputGain.setGainDecibels(parameters.inputGain);
    oversamplingFactor = parameters.factorOversampling;
    for (auto& clipper : clippers) {
        clipper.setDryWetProportion(parameters.dryWetProportion);
        clipper.setClippingType(parameters.clippingType);
        clipper.setThreshold(parameters.ceiling);
    }
    outputGain.setGainDecibels(parameters.outputGain);
}

}  // namespace pe::processor
