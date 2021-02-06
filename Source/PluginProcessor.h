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
class MultiShaperAudioProcessor  :  public juce::AudioProcessor,
                                    public waveshaping::OversampledWaveShaperListener
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
    
    //==============================================================================
    void onPostInputGain (juce::AudioBuffer<float>& buffer) override
    {
        inputMeterSource.measureBlock (buffer);
        setDecibelsIn (buffer.getMagnitude (0, buffer.getNumSamples()));
    }
    void onPostCeiling (juce::AudioBuffer<float>& buffer) override
    {
        ceilingMeterSource.measureBlock (buffer);
        setDecibelsClipped (buffer.getMagnitude (0, buffer.getNumSamples()));
    }
    void onPostOutputGain (juce::AudioBuffer<float>& buffer) override
    {
        outputMeterSource.measureBlock (buffer);
        setDecibelsOut (buffer.getMagnitude (0, buffer.getNumSamples()));
    }
    
    //==============================================================================
    foleys::LevelMeterSource& getInputMeterSource()   { return inputMeterSource; }
    foleys::LevelMeterSource& getOutputMeterSource()  { return outputMeterSource; }
    foleys::LevelMeterSource& getCeilingMeterSource() { return ceilingMeterSource; }
    
    //==============================================================================
    float getDecibelsIn()      { return decibelsIn; }
    float getDecibelsOut()     { return decibelsOut; }
    float getDecibelsClipped() { return decibelsClipped; }

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
    std::atomic<float> decibelsIn      { 0.0f };
    std::atomic<float> decibelsClipped { 0.0f };
    std::atomic<float> decibelsOut     { 0.0f };
    
    void setDecibelsIn (const float magnitude)
    {
        const float dbIn = magnitudeToDecibels (magnitude);
        decibelsIn = std::isinf(dbIn) ? 0.0f : dbIn;
    }
    
    void setDecibelsClipped (const float magnitude)
    {
        const float dbClipped = magnitudeToDecibels (magnitude);
        decibelsClipped = std::isinf(dbClipped) ? 0.0f : decibelsIn - dbClipped;
    }
    
    void setDecibelsOut (const float magnitude)
    {
        const auto dbOut = magnitudeToDecibels (magnitude);
        decibelsOut = std::isinf(dbOut) ? 0.0f : dbOut;
    }
    
    float magnitudeToDecibels (const float magnitude) const noexcept
    {
        return 20.0f * std::log10 (magnitude);
    }
    
    //==============================================================================
    std::shared_ptr<waveshaping::OversampledWaveShaper<float>> waveShaper;
    controller::WaveShaperController<float> waveShaperController;
    
    //==============================================================================
    foleys::LevelMeterSource inputMeterSource;
    foleys::LevelMeterSource ceilingMeterSource;
    foleys::LevelMeterSource outputMeterSource;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiShaperAudioProcessor)
};
