/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    waveShaper(new waveshaping::OversampledWaveShaper<float>()),
    waveShaperController(waveShaper)
{
    addParameter (inputGain = new juce::AudioParameterFloat("inputgain",
                                                        "Input Gain",
                                                        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                        0.0f)
                  );
    addParameter (outputGain = new juce::AudioParameterFloat("outputgain",
                                                        "Output Gain",
                                                        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
                                                        0.0f)
                  );
    addParameter (linkInOut = new juce::AudioParameterBool("linkinout",
                                                        "Link input & output gain",
                                                        false)
                  );
    addParameter (ceiling = new juce::AudioParameterFloat("ceiling",
                                                        "Ceiling",
                                                        juce::NormalisableRange<float>(-40.0f, -0.1f, 0.1f),
                                                        -0.1f)
                  );
    addParameter (clippingType = new juce::AudioParameterChoice("clippingtype",
                                                        "Clipping Type",
                                                        juce::StringArray{ "Logarythmic", "Hard", "Quintic", "Cubic Basic", "Hyperbolic Tan", "Algebraic", "Arctangent", "Sin", "Limit" },
                                                        0)
                  );
    addParameter (oversampleRate = new juce::AudioParameterChoice("oversamplerate",
                                                        "Oversampling Rate",
                                                        juce::StringArray{ "x2", "x4", "x8", "x16" },
                                                        0)
                  );
}

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
}

void MultiShaperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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
    
    waveShaperController.handleParametersChange({
        *linkInOut,
        *inputGain,
        *outputGain,
        *ceiling,
        clippingType->getIndex(),
        oversampleRate->getIndex()
    });
    waveShaper->process(buffer);
}

//==============================================================================
bool MultiShaperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MultiShaperAudioProcessor::createEditor()
{
    return new MultiShaperAudioProcessorEditor (*this);
}

//==============================================================================
void MultiShaperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MultiShaperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MultiShaperAudioProcessor();
}
