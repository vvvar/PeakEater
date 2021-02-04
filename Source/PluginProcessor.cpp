/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    using NormalisableRange = juce::NormalisableRange<float>;
    return
    {
        std::make_unique<juce::AudioParameterFloat> (Parameters::InputGain.Id,
                                                    Parameters::InputGain.Label,
                                                    NormalisableRange (-24.0f, 24.0f, 0.1f),
                                                    0.0f),
        std::make_unique<juce::AudioParameterFloat> (Parameters::OutputGain.Id,
                                                    Parameters::OutputGain.Label,
                                                    NormalisableRange (-24.0f, 24.0f, 0.1f),
                                                    0.0f),
        std::make_unique<juce::AudioParameterBool> (Parameters::LinkInOut.Id,
                                                   Parameters::LinkInOut.Label,
                                                   false),
        std::make_unique<juce::AudioParameterFloat> (Parameters::Ceiling.Id,
                                                    Parameters::Ceiling.Label,
                                                    NormalisableRange (-36.0f, -0.1f, 0.1f),
                                                    -0.1f),
        std::make_unique<juce::AudioParameterChoice> (Parameters::ClippingType.Id,
                                                     Parameters::ClippingType.Label,
                                                     juce::StringArray {
                                                        "Logarythmic",
                                                        "Hard",
                                                        "Quintic",
                                                        "Cubic Basic",
                                                        "Hyperbolic Tan",
                                                        "Algebraic",
                                                        "Arctangent",
                                                        "Sin",
                                                        "Limit"
                                                    },
                                                     0),
        std::make_unique<juce::AudioParameterChoice> (Parameters::OversampleRate.Id,
                                                     Parameters::OversampleRate.Label,
                                                     juce::StringArray { "x2", "x4", "x8", "x16" },
                                                     0)
    };
}

//==============================================================================
MultiShaperAudioProcessor::MultiShaperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters (*this, nullptr, juce::Identifier (JucePlugin_Name), createParameterLayout()),
    inputGain (static_cast<juce::AudioParameterFloat*> (parameters.getParameter (Parameters::InputGain.Id))),
    outputGain (static_cast<juce::AudioParameterFloat*> (parameters.getParameter (Parameters::OutputGain.Id))),
    linkInOut (static_cast<juce::AudioParameterBool*> (parameters.getParameter (Parameters::LinkInOut.Id))),
    ceiling (static_cast<juce::AudioParameterFloat*> (parameters.getParameter (Parameters::Ceiling.Id))),
    clippingType (static_cast<juce::AudioParameterChoice*> (parameters.getParameter (Parameters::ClippingType.Id))),
    oversampleRate (static_cast<juce::AudioParameterChoice*> (parameters.getParameter (Parameters::OversampleRate.Id))),
    waveShaper (new waveshaping::OversampledWaveShaper<float>()),
    waveShaperController (waveShaper)
{}

MultiShaperAudioProcessor::~MultiShaperAudioProcessor()
{
}

//==============================================================================
const juce::String MultiShaperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MultiShaperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MultiShaperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MultiShaperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MultiShaperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MultiShaperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MultiShaperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MultiShaperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MultiShaperAudioProcessor::getProgramName (int index)
{
    return {};
}

void MultiShaperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MultiShaperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    waveShaper->prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2 });
    waveShaperController.setup({
        *linkInOut,
        *inputGain,
        *outputGain,
        *ceiling,
        *clippingType,
        *oversampleRate
    });
}

void MultiShaperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    waveShaper->reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MultiShaperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MultiShaperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
    waveShaperController.handleParametersChange ({
        *linkInOut,
        *inputGain,
        *outputGain,
        *ceiling,
        *clippingType,
        *oversampleRate
    });
    waveShaper->process (buffer);
}

//==============================================================================
bool MultiShaperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultiShaperAudioProcessor::createEditor()
{
    return new MultiShaperAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void MultiShaperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MultiShaperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
     
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultiShaperAudioProcessor();
}
