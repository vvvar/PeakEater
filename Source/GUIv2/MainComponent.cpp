#include "MainComponent.h"

namespace pe
{
namespace gui
{
MainComponent::MainComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                              std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                              std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                              std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
    : juce::Component()
    , mInputLevelMeter (inputLevelMeter)
    , mOutputLevelMeter (outputLevelMeter)
    , mClipMeter (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
{
    addAndMakeVisible (mInputLevelMeter);
    addAndMakeVisible (mOutputLevelMeter);
    addAndMakeVisible (mClipMeter);
}

void MainComponent::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (6)), Track (Fr (1)) };
    grid.items = { Item (mInputLevelMeter), Item (mClipMeter), Item (mOutputLevelMeter) };
    grid.performLayout (getLocalBounds());
}
} // namespace gui
} // namespace pe