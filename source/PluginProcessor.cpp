#include "PluginProcessor.h"

#include "Parameters.h"
#include "PluginEditor.h"

//==============================================================================
namespace pe::processor {

namespace {
[[nodiscard]] ClippingType parameterChoiceToClippingType(int const &parameterChoice) {
    ClippingType clippingType = ClippingType::HARD;
    switch (parameterChoice) {
        case 0:
            clippingType = ClippingType::HARD;
            break;
        case 1:
            clippingType = ClippingType::QUINTIC;
            break;
        case 2:
            clippingType = ClippingType::CUBIC;
            break;
        case 3:
            clippingType = ClippingType::HYPERBOLIC_TAN;
            break;
        case 4:
            clippingType = ClippingType::ALGEBRAIC;
            break;
        case 5:
            clippingType = ClippingType::ARCTANGENT;
            break;
        default:
            clippingType = ClippingType::HARD;
            break;
    }
    return clippingType;
}
//-----------------------------------------------------------
// Return true if a and b almost equal(with tolerance)
bool cmpf(float const a, float const b) {
    float const epsilon = 0.005f;
    return (::fabs(a - b) < epsilon);
}
}  // namespace

PeakEaterAudioProcessor::PeakEaterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
      mParameters(std::make_shared<juce::AudioProcessorValueTreeState>(
          *this, nullptr, juce::Identifier(JucePlugin_Name), pe::params::ParametersProvider::getInstance().createParameterLayout())),
      mInputGain(static_cast<juce::AudioParameterFloat *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getInputGain().getId().getParamID()))),
      mOutputGain(static_cast<juce::AudioParameterFloat *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getOutputGain().getId().getParamID()))),
      mLinkInOut(static_cast<juce::AudioParameterBool *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getLinkInOut().getId().getParamID()))),
      mBypass(static_cast<juce::AudioParameterBool *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getBypass().getId().getParamID()))),
      mCeiling(static_cast<juce::AudioParameterFloat *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getCeiling().getId().getParamID()))),
      mClippingType(static_cast<juce::AudioParameterChoice *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getClippingType().getId().getParamID()))),
      mOversampleRate(static_cast<juce::AudioParameterChoice *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getOversampleRate().getId().getParamID()))),
      mDryWet(static_cast<juce::AudioParameterFloat *>(
          mParameters->getParameter(pe::params::ParametersProvider::getInstance().getDryWet().getId().getParamID()))),
      mLevelMeterPostIn(std::make_shared<processor::LevelMeter<float>>()),
      mLevelMeterPostClipper(std::make_shared<processor::LevelMeter<float>>()),
      mLevelMeterPostOut(std::make_shared<processor::LevelMeter<float>>()),
      mPluginSizeState({mPluginSizeConstraints.minWidth, mPluginSizeConstraints.minHeight}) {
}

PeakEaterAudioProcessor::~PeakEaterAudioProcessor() {}

//==============================================================================
const juce::String PeakEaterAudioProcessor::getName() const { return JucePlugin_Name; }

bool PeakEaterAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PeakEaterAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PeakEaterAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PeakEaterAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int PeakEaterAudioProcessor::getNumPrograms() {
    // NB: some hosts don't cope very well if you tell them there are 0
    // programs,
    // so this should be at least 1, even if you're not really implementing
    // programs.
    return 1;
}

int PeakEaterAudioProcessor::getCurrentProgram() { return 1; }

void PeakEaterAudioProcessor::setCurrentProgram(int /* index */) {
    // Since we are not supporting presets
}

const juce::String PeakEaterAudioProcessor::getProgramName(int /* index */) { return "default"; }

void PeakEaterAudioProcessor::changeProgramName(int /* index */, juce::String const & /* newName */) {
    // Since we are not supporting presets
}

void PeakEaterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec const spec{sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2};
    inputGain.prepare(spec);
    for (auto &clipper : clippers) {
        clipper.prepare(spec);
    }
    outputGain.prepare(spec);
}

void PeakEaterAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    outputGain.reset();
    for (auto &clipper : clippers) {
        clipper.reset();
    }
    inputGain.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PeakEaterAudioProcessor::isBusesLayoutSupported(BusesLayout const &layouts) const {
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo()) {
        return false;
    }

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) {
        return false;
    }
#endif

    return true;
#endif
}
#endif

void PeakEaterAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer & /* midiMessages */) {
    juce::ScopedNoDenormals const noDenormals;
    //-----------------------------------------------------------
    // Handle link-in-out
    if (*mLinkInOut) {
        //-----------------------------------------------------------
        // Linking means that Input and Output are having opposite values.
        // For ex. when input = -16 then output = +16.
        // First - detect which value has changed, it could be only one at time
        float const nextInputGainValue = *mInputGain;
        float const nextOutputGainValue = *mOutputGain;
        float const currentInGainValue = inputGain.getGainDecibels();
        float const currentOutGainValue = outputGain.getGainDecibels();
        if (!cmpf(nextInputGainValue, currentInGainValue)) {
            //-----------------------------------------------------------
            // Input gain changed - link output with it
            *mOutputGain = -nextInputGainValue;
        } else if (!cmpf(nextOutputGainValue, currentOutGainValue)) {
            //-----------------------------------------------------------
            // Output gain changed - link input with it
            *mInputGain = -nextOutputGainValue;
        } else if (!cmpf(currentInGainValue, currentOutGainValue)) {
            //-----------------------------------------------------------
            // Nor input not output has changed. but link is on and
            // current input and output are still different. Means we
            // have turned link-in-out ON but haven't moved any slider yet,
            // so we have to sync then now
            *mOutputGain = -nextInputGainValue;
        } else {
            // Already linked - do nothing
        }
    }
    //-----------------------------------------------------------
    // Propagate params to DSP processors
    inputGain.setGainDecibels(*mInputGain);
    for (auto &clipper : clippers) {
        clipper.setClippingType(parameterChoiceToClippingType(*mClippingType));
        clipper.setThreshold(*mCeiling);
        clipper.setDryWetProportion(*mDryWet);
    }
    outputGain.setGainDecibels(*mOutputGain);
    //-----------------------------------------------------------
    // Do actual DSP
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing const context(audioBlock);
    if (!*mBypass) {
        inputGain.process(context);
        mLevelMeterPostIn->updateLevels(context.getOutputBlock());
        for (auto &clipper : clippers) {
            if (clipper.getOversamplingFactor() == static_cast<size_t>(*mOversampleRate)) {
                clipper.process(context);
            }
        }
        mLevelMeterPostClipper->updateLevels(context.getOutputBlock());
        outputGain.process(context);
        mLevelMeterPostOut->updateLevels(context.getOutputBlock());
    }
}

bool PeakEaterAudioProcessor::hasEditor() const {
    // change this to false if you choose to not supply an editor
    return true;
}

juce::AudioProcessorEditor *PeakEaterAudioProcessor::createEditor() {
    return new pe::PeakEaterAudioProcessorEditor(
        *this, mParameters,
        {.inputLevelMeter = mLevelMeterPostIn, .clippingLevelMeter = mLevelMeterPostClipper, .outputLevelMeter = mLevelMeterPostOut});
}

void PeakEaterAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto const state = mParameters->copyState();
    std::unique_ptr<juce::XmlElement> const xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PeakEaterAudioProcessor::setStateInformation(void const *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    std::unique_ptr<juce::XmlElement> const xmlState(getXmlFromBinary(data, sizeInBytes));

    if ((xmlState.get() != nullptr) && (xmlState->hasTagName(mParameters->state.getType()))) {
        mParameters->replaceState(juce::ValueTree::fromXml(*xmlState));
    }
}

PeakEaterAudioProcessor::PluginSizeConstraints PeakEaterAudioProcessor::getPluginSizeConstraints() const { return mPluginSizeConstraints; }

void PeakEaterAudioProcessor::setPluginSizeState(PluginSizeState const &&pluginSizeState) { mPluginSizeState = pluginSizeState; }

PeakEaterAudioProcessor::PluginSizeState PeakEaterAudioProcessor::getPluginSizeState() const { return mPluginSizeState; }

}  // namespace pe::processor

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new pe::processor::PeakEaterAudioProcessor(); }
