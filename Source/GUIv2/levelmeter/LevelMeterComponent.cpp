#include "LevelMeterComponent.h"

#include <juce_graphics/juce_graphics.h>

namespace pe
{
namespace gui
{
LevelMeterComponent::LevelMeterComponent (std::shared_ptr<pe::dsp::LevelMeter<float>> levelMeter)
    : juce::Component()
    , mMeterTimer (std::bind (&LevelMeterComponent::onTimerTick, this))
    , mLevelMeter (levelMeter)
    , mDial()
{
    mMeters.setChannelFormat (juce::AudioChannelSet::stereo());
    sd::SoundMeter::Options meterOptions;
    meterOptions.faderEnabled = false;
    meterOptions.headerEnabled = false;
    meterOptions.peakSegment_db = -3.0f;
    meterOptions.warningSegment_db = -12.0f;
    mMeters.setOptions (meterOptions);
    addAndMakeVisible (mMeters);

    addAndMakeVisible (mDial);

    // mSLider.setTextBoxIsEditable (false);
    // mSLider.setSliderSnapsToMousePosition (false);
    // mSLider.setScrollWheelEnabled (true);
    // mSLider.setRange (-36.0f, 36.0f);
    // mSLider.setTextValueSuffix (" dB");
    // mSLider.setValue (0.0f);
    // addAndMakeVisible (mSLider);

    mMeterTimer.startTimer (100);
}

LevelMeterComponent::~LevelMeterComponent()
{
    setLookAndFeel (nullptr);
}

void LevelMeterComponent::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
    grid.items = {
        // Item (mSLider),
        Item (mMeters),
        Item (mDial),
        // Item (mSLider),
        Item (mMeters)
    };
    grid.performLayout (getLocalBounds());
}

void LevelMeterComponent::onTimerTick()
{
    float amplificationL;
    float amplificationR;
    std::tie (amplificationL, amplificationR) = mLevelMeter->getAmplification();
    mMeters.setInputLevel (0, amplificationL);
    mMeters.setInputLevel (1, amplificationR);
}
} // namespace gui
} // namespace pe