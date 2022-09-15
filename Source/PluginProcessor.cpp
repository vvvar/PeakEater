/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Parameters.h"

//==============================================================================
PeakEaterAudioProcessor::PeakEaterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
#endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                          )
    ,
#endif
    mParameters (std::make_shared<juce::AudioProcessorValueTreeState> (*this, nullptr, juce::Identifier (JucePlugin_Name), pe::params::ParametersProvider::getInstance().createParameterLayout()))
    , mInputGain (static_cast<juce::AudioParameterFloat*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getInputGain().getId())))
    , mOutputGain (static_cast<juce::AudioParameterFloat*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getOutputGain().getId())))
    , mLinkInOut (static_cast<juce::AudioParameterBool*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getLinkInOut().getId())))
    , mBypass (static_cast<juce::AudioParameterBool*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getBypass().getId())))
    , mCeiling (static_cast<juce::AudioParameterFloat*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getCeiling().getId())))
    , mClippingType (static_cast<juce::AudioParameterChoice*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getClippingType().getId())))
    , mOversampleRate (static_cast<juce::AudioParameterChoice*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getOversampleRate().getId())))
    , mWaveShaperController()
    , mLevelMeterPostIn (std::make_shared<pe::dsp::LevelMeter<float>>())
    , mLevelMeterPostClipper (std::make_shared<pe::dsp::LevelMeter<float>>())
    , mLevelMeterPostOut (std::make_shared<pe::dsp::LevelMeter<float>>())
{
}

PeakEaterAudioProcessor::~PeakEaterAudioProcessor()
{
}

//==============================================================================
const juce::String PeakEaterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PeakEaterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PeakEaterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PeakEaterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PeakEaterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PeakEaterAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int PeakEaterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PeakEaterAudioProcessor::setCurrentProgram (int /* index */)
{
}

const juce::String PeakEaterAudioProcessor::getProgramName (int /* index */)
{
    return {};
}

void PeakEaterAudioProcessor::changeProgramName (int /* index */, const juce::String& /* newName */)
{
}

//==============================================================================
void PeakEaterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mWaveShaperController.onPostInputGain ([this](auto& buffer) -> auto {
        mLevelMeterPostIn->updateLevels (buffer);
    });
    mWaveShaperController.onPostCeiling ([this](auto& buffer) -> auto {
        mLevelMeterPostClipper->updateLevels (buffer);
    });
    mWaveShaperController.onPostOutputGain ([this](auto& buffer) -> auto {
        mLevelMeterPostOut->updateLevels (buffer);
    });
    mWaveShaperController.prepare ({ *mInputGain,
                                     *mOutputGain,
                                     *mCeiling,
                                     *mClippingType,
                                     *mOversampleRate },
                                   { sampleRate, static_cast<juce::uint32> (samplesPerBlock), 2 });
}

void PeakEaterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    mWaveShaperController.unsubscribeFromAll();
    mWaveShaperController.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PeakEaterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void PeakEaterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /* midiMessages */)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    if (*mLinkInOut)
    {
        float const inGainValue = *mInputGain;
        float const outGainValue = *mOutputGain;
        float const prevInGainValue = mWaveShaperController.getPreviousInputGain();
        float const prevOutGainValue = mWaveShaperController.getPreviousOutputGain();
        if (inGainValue != prevInGainValue) // Input gain changed, sync output
        {
            *mOutputGain = -inGainValue;
        }
        else if (outGainValue != prevOutGainValue) // Output gain changed, sync input
        {
            *mInputGain = -outGainValue;
        }
        else // nothing chnaged, sync out with in
        {
            *mOutputGain = -inGainValue;
        }
    }
    mWaveShaperController.handleParametersChange ({ *mInputGain,
                                                    *mOutputGain,
                                                    *mCeiling,
                                                    *mClippingType,
                                                    *mOversampleRate });
    if (! *mBypass)
    {
        mWaveShaperController.process (buffer);
    }
}

void PeakEaterAudioProcessor::processBlockBypassed (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
}

juce::AudioProcessorParameter* PeakEaterAudioProcessor::getBypassParameter() const
{
    return mBypass;
}

//==============================================================================
bool PeakEaterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PeakEaterAudioProcessor::createEditor()
{
    return new pe::PeakEaterAudioProcessorEditor (*this, mParameters, mLevelMeterPostIn, mLevelMeterPostClipper, mLevelMeterPostOut);
}

//==============================================================================
void PeakEaterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = mParameters->copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PeakEaterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (mParameters->state.getType()))
        {
            mParameters->replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PeakEaterAudioProcessor();
}
