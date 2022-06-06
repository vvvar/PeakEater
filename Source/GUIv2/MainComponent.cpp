#include "MainComponent.h"

namespace pe
{
namespace gui
{
MainComponent::MainComponent (std::shared_ptr<foleys::LevelMeterSource> levelMeterSource)
    : juce::Component()
    , mLevelMeter (foleys::LevelMeter::MeterFlags::Default)
{
    mLevelMeter.setMeterSource (levelMeterSource.get());
    addAndMakeVisible (mLevelMeter);
}

void MainComponent::paint (juce::Graphics& g)
{
    // g.fillAll (juce::Colours::lightblue);
    // g.setColour (juce::Colours::darkblue);
}

void MainComponent::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)) };
    grid.items = { Item (mLevelMeter) };
    grid.performLayout (getLocalBounds());
}
} // namespace gui
} // namespace pe