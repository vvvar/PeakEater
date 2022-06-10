#include "MainComponent.h"

namespace pe
{
namespace gui
{
MainComponent::MainComponent (std::shared_ptr<pe::dsp::LevelMeter<float>> levelMeter)
    : juce::Component()
    , mMeterTimer (std::bind (&MainComponent::onTimerTick, this))
    , mLevelMeter (levelMeter)
{
    mMeters.setChannelFormat (juce::AudioChannelSet::stereo());
    sd::SoundMeter::Options meterOptions;
    meterOptions.faderEnabled = false;
    meterOptions.headerEnabled = false;
    meterOptions.peakSegment_db = -3.0f;
    meterOptions.warningSegment_db = -12.0f;
    mMeters.setOptions (meterOptions);
    addAndMakeVisible (mMeters);

    mMeterTimer.startTimer (100);
}

void MainComponent::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)) };
    grid.items = { Item (mMeters) };
    grid.performLayout (getLocalBounds());
}

void MainComponent::onTimerTick()
{
    float amplificationL;
    float amplificationR;
    std::tie (amplificationL, amplificationR) = mLevelMeter->getAmplification();
    mMeters.setInputLevel (0, amplificationL);
    mMeters.setInputLevel (1, amplificationR);
}
} // namespace gui
} // namespace pe