#include "CentralPanel.h"

#include "LevelMetersPack.h"

namespace pe::gui {
CentralPanel::CentralPanel(std::shared_ptr<juce::AudioProcessorValueTreeState> parameters, LevelMetersPack const& levelMetersPack,
                           std::shared_ptr<Ticks> ticks)
    : juce::Component(),
      mClipMeter(parameters, levelMetersPack, ticks),
      mControlPanel(parameters, levelMetersPack),
      mLinkingPanel(parameters),
      mAnalyserComponent(parameters, levelMetersPack),
      mScalingSwitch(ticks) {
    addAndMakeVisible(mClipMeter);
    addAndMakeVisible(mControlPanel);
    addAndMakeVisible(mLinkingPanel);
    addAndMakeVisible(mAnalyserComponent);
    addAndMakeVisible(mScalingSwitch);
}

void CentralPanel::resized() {
    auto const localBounds = getLocalBounds();

    mClipMeter.setBounds(localBounds);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = {Track(Fr(3)), Track(Fr(2))};
    grid.templateColumns = {Track(Fr(1))};
    grid.items = {Item(mControlPanel), Item(mLinkingPanel).withMargin(Item::Margin(0, localBounds.proportionOfWidth(0.13f), 0, 0))};
    auto const toRemoveFromTop = localBounds.proportionOfHeight(0.65f);
    auto const toRemoveFromSides = localBounds.proportionOfWidth(0.14f);
    grid.performLayout(localBounds.withTrimmedTop(toRemoveFromTop).withTrimmedLeft(toRemoveFromSides).withTrimmedRight(toRemoveFromSides));

    mAnalyserComponent.setBounds(localBounds.withWidth(localBounds.proportionOfWidth(0.28f))
                                     .withHeight(localBounds.proportionOfHeight(0.155f))
                                     .withX(localBounds.proportionOfWidth(0.1f))
                                     .withY(localBounds.proportionOfHeight(0.05f)));

    mScalingSwitch.setBounds(localBounds.withWidth(localBounds.proportionOfWidth(0.065f))
                                 .withHeight(localBounds.proportionOfWidth(0.065f))
                                 .withX(localBounds.proportionOfWidth(0.875f))
                                 .withY(localBounds.proportionOfHeight(0.71f)));
}
}  // namespace pe::gui
