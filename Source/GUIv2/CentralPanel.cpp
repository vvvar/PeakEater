#include "CentralPanel.h"

namespace pe
{
namespace gui
{
CentralPanel::CentralPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                            std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                            std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
    : juce::Component()
    , mClipMeter (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
    , mControlPanel (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
    , mLinkingPanel (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
{
    addAndMakeVisible (mClipMeter);
    addAndMakeVisible (mControlPanel);
    addAndMakeVisible (mLinkingPanel);
}

void CentralPanel::resized()
{
    auto const localBounds = getLocalBounds();

    mClipMeter.setBounds (localBounds);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (3)), Track (Fr (2)) };
    grid.templateColumns = { Track (Fr (1)) };
    grid.items = { Item (mControlPanel), Item (mLinkingPanel) };
    auto const toRemoveFromTop = localBounds.proportionOfHeight (0.7f);
    auto const toRemoveFromSides = localBounds.proportionOfWidth (0.05f);
    grid.performLayout (localBounds
                            .withTrimmedTop (toRemoveFromTop)
                            .withTrimmedLeft (toRemoveFromSides)
                            .withTrimmedRight (toRemoveFromSides));
}
} // namespace gui
} // namespace pe