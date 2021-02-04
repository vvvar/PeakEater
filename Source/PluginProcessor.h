/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveShaping/OversampledWaveShaper.h"
#include "WaveShaperController.h"

//==============================================================================
/*
 * Helper namespace to keep consistent parameter info
 * across all project. For example, to keep label text similar
 * in the automation section and in the UI.
 */
namespace Parameters
{

struct ParameterInfo
{
    juce::String Id;
    juce::String Label;
};

static const ParameterInfo InputGain      { "InputGain", "Input Gain" };
static const ParameterInfo OutputGain     { "OutputGain", "Output Gain" };
static const ParameterInfo LinkInOut      { "LinkInOut", "Link In & Out" };
static const ParameterInfo Ceiling        { "Ceiling", "Ceiling" };
static const ParameterInfo ClippingType   { "ClippingType", "Algorythm" };
static const ParameterInfo OversampleRate { "OversampleRate", "Oversample" };

}

//==============================================================================
/**
*/
class MultiShaperAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MultiShaperAudioProcessor();
    ~MultiShaperAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    juce::AudioParameterFloat*  inputGain;
    juce::AudioParameterFloat*  outputGain;
    juce::AudioParameterBool*   linkInOut;
    juce::AudioParameterFloat*  ceiling;
    juce::AudioParameterChoice* clippingType;
    juce::AudioParameterChoice* oversampleRate;
    
    //==============================================================================
    std::shared_ptr<waveshaping::OversampledWaveShaper<float>> waveShaper;
    controller::WaveShaperController<float> waveShaperController;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiShaperAudioProcessor)
};
