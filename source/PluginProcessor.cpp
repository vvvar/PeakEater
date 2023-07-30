#include "PluginProcessor.h"

#include "Parameters.h"
#include "PluginEditor.h"

//==============================================================================
namespace {
pe::processor::ClippingType parameterChoiceToClippingType(int const &parameterChoice) {
    switch (parameterChoice) {
        case 0:
            return pe::processor::ClippingType::HARD;
        case 1:
            return pe::processor::ClippingType::QUINTIC;
        case 2:
            return pe::processor::ClippingType::CUBIC;
        case 3:
            return pe::processor::ClippingType::HYPERBOLIC_TAN;
        case 4:
            return pe::processor::ClippingType::ALGEBRAIC;
        case 5:
            return pe::processor::ClippingType::ARCTANGENT;
        default:
            return pe::processor::ClippingType::HARD;
    }
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
      mLevelMeterPostIn(std::make_shared<pe::dsp::LevelMeter<float>>()),
      mLevelMeterPostClipper(std::make_shared<pe::dsp::LevelMeter<float>>()),
      mLevelMeterPostOut(std::make_shared<pe::dsp::LevelMeter<float>>()),
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
    return 1;  // NB: some hosts don't cope very well if you tell them there are 0
               // programs,
               // so this should be at least 1, even if you're not really implementing
               // programs.
}

int PeakEaterAudioProcessor::getCurrentProgram() { return 1; }

void PeakEaterAudioProcessor::setCurrentProgram(int /* index */) {}

const juce::String PeakEaterAudioProcessor::getProgramName(int /* index */) { return "default"; }

void PeakEaterAudioProcessor::changeProgramName(int /* index */, juce::String const & /* newName */) {}

//==============================================================================
void PeakEaterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    juce::dsp::ProcessSpec const spec{sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2};
    pe::processor::MainProcessor::EvenHandlers const handlers{
        .postInput = std::bind_front(&pe::dsp::LevelMeter<float>::updateLevels, mLevelMeterPostIn),
        .postClipping = std::bind_front(&pe::dsp::LevelMeter<float>::updateLevels, mLevelMeterPostClipper),
        .postOutput = std::bind_front(&pe::dsp::LevelMeter<float>::updateLevels, mLevelMeterPostOut)};
    mainProcessor.prepare(spec, handlers);
}

void PeakEaterAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    mainProcessor.reset();
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
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) return false;
#endif

    return true;
#endif
}
#endif

void PeakEaterAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer & /* midiMessages */) {
    juce::ScopedNoDenormals const noDenormals;

    mainProcessor.updateParameters({.inputGain = *mInputGain,
                                    .factorOversampling = static_cast<size_t>(*mOversampleRate),
                                    .clippingType = parameterChoiceToClippingType(*mClippingType),
                                    .ceiling = *mCeiling,
                                    .outputGain = *mOutputGain,
                                    .dryWetProportion = *mDryWet});

    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing const context(audioBlock);
    if (!*mBypass) {
        mainProcessor.process(context);
    }
}

//==============================================================================
bool PeakEaterAudioProcessor::hasEditor() const {
    // change this to false if you choose to not supply an editor
    return true;
}

juce::AudioProcessorEditor *PeakEaterAudioProcessor::createEditor() {
    return new pe::PeakEaterAudioProcessorEditor(
        *this, mParameters,
        {.inputLevelMeter = mLevelMeterPostIn, .clippingLevelMeter = mLevelMeterPostClipper, .outputLevelMeter = mLevelMeterPostOut});
}

//==============================================================================
void PeakEaterAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = mParameters->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PeakEaterAudioProcessor::setStateInformation(void const *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(mParameters->state.getType())) {
            mParameters->replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

PeakEaterAudioProcessor::PluginSizeConstraints PeakEaterAudioProcessor::getPluginSizeConstraints() const { return mPluginSizeConstraints; }

void PeakEaterAudioProcessor::setPluginSizeState(PluginSizeState const &&pluginSizeState) { mPluginSizeState = pluginSizeState; }

PeakEaterAudioProcessor::PluginSizeState PeakEaterAudioProcessor::getPluginSizeState() const { return mPluginSizeState; }

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new PeakEaterAudioProcessor(); }
