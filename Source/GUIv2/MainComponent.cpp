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
    , mClipMeter (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
{
    addAndMakeVisible (mClipMeter);
}

void MainComponent::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)) };
    grid.items = { Item (mClipMeter) };
    grid.performLayout (getLocalBounds());
}
} // namespace gui
} // namespace pe