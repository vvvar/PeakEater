#include "PluginEditor.h"

namespace pe
{
//==============================================================================
PeakEaterAudioProcessorEditor::PeakEaterAudioProcessorEditor (
    PeakEaterAudioProcessor& audioProcessor,
    std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
    std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
    std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
    std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
    : juce::AudioProcessorEditor (audioProcessor),
      mMainComponent (parameters,
                      inputLevelMeter,
                      clippingLevelMeter,
                      outputLevelMeter),
      mAudioProcessor (audioProcessor)
{
    auto const size = mAudioProcessor.getPluginSizeState();
    auto const constraints = mAudioProcessor.getPluginSizeConstraints();

    addAndMakeVisible (mMainComponent);

    setSize (size.width, size.height);
    setResizable (true, true);

    setResizeLimits (constraints.minWidth,
                     constraints.minHeight,
                     constraints.maxWidth,
                     constraints.maxHeight);
    getConstrainer()->setFixedAspectRatio (constraints.aspectRatio);
}

PeakEaterAudioProcessorEditor::~PeakEaterAudioProcessorEditor()
{
    // Call to destructor indicates plugin window has been closed
    // We are saving it's state to keep resized with/height
    mAudioProcessor.setPluginSizeState ({ getWidth(), getHeight() });
}

//==============================================================================
void PeakEaterAudioProcessorEditor::paint (juce::Graphics& g) { g.fillAll(); }

void PeakEaterAudioProcessorEditor::resized()
{
    mMainComponent.setBounds (getLocalBounds());
}
} // namespace pe
