#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include <memory>

#include "processor/Clipper.h"
#include "processor/LevelMeter.h"

namespace pe::processor {

class PeakEaterAudioProcessor : public juce::AudioProcessor {
   public:
    PeakEaterAudioProcessor();
    ~PeakEaterAudioProcessor() override;

    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(BusesLayout const& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, juce::String const& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(void const* data, int sizeInBytes) override;

    // Hack to preserve plugin size
    struct PluginSizeConstraints {
        int const minWidth = 640;
        int const minHeight = 400;
        int const maxWidth = 3840;
        int const maxHeight = 2400;
        double const aspectRatio = 16.0 / 10.0;
    };
    struct PluginSizeState {
        int width;
        int height;
    };
    PluginSizeConstraints getPluginSizeConstraints() const;
    void setPluginSizeState(PluginSizeState const&& pluginSizeState);
    PluginSizeState getPluginSizeState() const;

   private:
    std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
    juce::AudioParameterFloat* mInputGain;
    juce::AudioParameterFloat* mOutputGain;
    juce::AudioParameterBool* mLinkInOut;
    juce::AudioParameterBool* mBypass;
    juce::AudioParameterFloat* mCeiling;
    juce::AudioParameterChoice* mClippingType;
    juce::AudioParameterChoice* mOversampleRate;
    juce::AudioParameterFloat* mDryWet;

    juce::dsp::DryWetMixer<float> dryWet{44};
    juce::dsp::Gain<float> inputGain;
    std::array<Clipper<float>, 6> clippers{Clipper<float>{0}, Clipper<float>{1}, Clipper<float>{2},
                                           Clipper<float>{3}, Clipper<float>{4}, Clipper<float>{5}};
    juce::dsp::Gain<float> outputGain;

    std::shared_ptr<processor::LevelMeter<float>> mLevelMeterPostIn;
    std::shared_ptr<processor::LevelMeter<float>> mLevelMeterPostClipper;
    std::shared_ptr<processor::LevelMeter<float>> mLevelMeterPostOut;

    PluginSizeConstraints mPluginSizeConstraints;
    PluginSizeState mPluginSizeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PeakEaterAudioProcessor)
};

}  // namespace pe::processor
