#include "LevelMeterComponent.h"

#include <juce_graphics/juce_graphics.h>

namespace pe
{
namespace gui
{
LevelMeterComponent::LevelMeterComponent (std::shared_ptr<pe::dsp::LevelMeter<float>> levelMeter, std::string const& name)
    : juce::Component()
    , mMeterTimer (std::bind (&LevelMeterComponent::onTimerTick, this))
    , mLevelMeter (levelMeter)
    , mLabel()
{
    sd::SoundMeter::Options meterOptions;
    meterOptions.faderEnabled = false;
    meterOptions.headerEnabled = false;
    //  meterOptions.useMinimalMode = true;
    meterOptions.tickMarksEnabled = false;
    meterOptions.tickMarks = { 0.0f, -3.0f, -5.0f, -8.0f, -11.0f, -14.0f, -17.0f, -20.0f, -23.0f, -26.0f, -29.0f, -32.0f, -36.0f };
    mMeters.setOptions (meterOptions);
    mMeters.setChannelFormat (juce::AudioChannelSet::stereo());
    mMeters.enableValue (false);
    // mMeters.showTickMarks (false);
    mMeters.setLabelStripPosition (sd::SoundMeter::LabelStripPosition::none);
    mMeters.setColour (sd::SoundMeter::MeterChannel::backgroundColourId, juce::Colour (22, 33, 62));
    mMeters.setColour (sd::SoundMeter::MeterChannel::mutedMouseOverColourId, juce::Colour (22, 33, 62));
    mMeters.setColour (sd::SoundMeter::MeterChannel::inactiveColourId, juce::Colour (22, 33, 62));
    mMeters.setColour (sd::SoundMeter::MeterChannel::peakColourId, juce::Colour (22, 33, 62));
    mMeters.setColour (sd::SoundMeter::MeterChannel::normalColourId, juce::Colour (22, 33, 62));
    mMeters.setColour (sd::SoundMeter::MeterChannel::tickMarkColourId, juce::Colour (22, 33, 62));
    mMeters.setColour (sd::SoundMeter::MeterChannel::textColourId, juce::Colour (22, 33, 62));
    mMeters.setColour (sd::SoundMeter::MeterChannel::faderColourId, juce::Colour (22, 33, 62));
    mMeterTimer.startTimer (20);
    addAndMakeVisible (mMeters);

    // mLabel.setColour (juce::Label::textColourId, juce::Colours::lightgreen);
    // mLabel.setJustificationType (juce::Justification::centred);
    // mLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    // mLabel.setText (name, juce::NotificationType::dontSendNotification);
    // addAndMakeVisible (mLabel);
}

LevelMeterComponent::~LevelMeterComponent()
{
    setLookAndFeel (nullptr);
}

void LevelMeterComponent::resized()
{
    // juce::Grid grid;
    // using Track = juce::Grid::TrackInfo;
    // using Fr = juce::Grid::Fr;
    // using Item = juce::GridItem;
    // grid.templateRows = { Track (Fr (1)), Track (Fr (10)) };
    // grid.templateColumns = { Track (Fr (1)) };
    // grid.items = { Item (mLabel), Item (mMeters) };
    // grid.performLayout (getLocalBounds());

    mMeters.setBounds (getLocalBounds());

    // juce::FlexBox fb;
    // fb.flexWrap = juce::FlexBox::Wrap::wrap;
    // fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;

    // juce::FlexItem left ((float) getWidth() / 4.0f, (float) getHeight(), leftPanel);
    // juce::FlexItem right ((float) getWidth() / 4.0f, (float) getHeight(), rightPanel);
    // juce::FlexItem main ((float) getWidth() / 2.0f, (float) getHeight(), mainPanel);

    // fb.items.addArray ({ juce::FlexItem (mMeters).withFlex (1) });
    // fb.performLayout (getLocalBounds().toFloat());
    // mMeters.setBounds (getLocalBounds());
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