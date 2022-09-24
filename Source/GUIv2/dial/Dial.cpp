#include "Dial.h"
#include "../Utils.h"
#include <juce_graphics/juce_graphics.h>

namespace pe
{
namespace gui
{
Dial::Dial (std::string const& labelText,
            std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
            std::string const& parameterId)
    : mAttachment (*parameters, parameterId, mSlider)
{
    mSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSlider.setTextBoxStyle (juce::Slider::TextBoxAbove, false, 80, 20);
    mSlider.setColour (juce::Slider::textBoxTextColourId, juce::Colours::black);
    mSlider.setColour (juce::Slider::textBoxTextColourId, juce::Colours::antiquewhite);
    mSlider.setColour (juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
    mSlider.setColour (juce::Slider::rotarySliderFillColourId, juce::Colours::blue);
    mSlider.setLookAndFeel (&mLookAndFeel);
    mSlider.setVelocityModeParameters (1.0, 1, 0.02, true, juce::ModifierKeys::shiftModifier);
    mSlider.setTextBoxIsEditable (false);
    // mSlider.setTextValueSuffix (" dB");

    // mSlider.setVelocityBasedMode (true);
    // mSlider.setVelocityModeParameters (1.0, 1, 0.05, false);
    // mSlider.setSkewFactor (1.8, true);
    // mSlider.setSkewFactorFromMidPoint (0.0);
    // mSlider.onValueChange = [&]()
    // {
    //     if (mSlider.getValue() < 10)
    //         mSlider.setNumDecimalPlacesToDisplay (2);
    //     else if (10 <= mSlider.getValue() && mSlider.getValue() < 100)
    //         mSlider.setNumDecimalPlacesToDisplay (1);
    //     else
    //         mSlider.setNumDecimalPlacesToDisplay (0);
    // };

    // mSlider.setSkewFactorFromMidPoint (0.0);
    // mSlider.setRange (0.0, 100.0, 0.01);
    // mSlider.setValue (50.0);
    // mSlider.setDoubleClickReturnValue (true, 50.0);
    // mSlider.setRotaryParameters (juce::MathConstants<float>::pi * 1.25f,
    //                              juce::MathConstants<float>::pi * 2.75f,
    //                              true);
    // mSlider.onValueChange = [&]()
    // {
    //     if (mSlider.getValue() < 10)
    //         mSlider.setNumDecimalPlacesToDisplay (2);
    //     else if (10 <= mSlider.getValue() && mSlider.getValue() < 100)
    //         mSlider.setNumDecimalPlacesToDisplay (1);
    //     else
    //         mSlider.setNumDecimalPlacesToDisplay (0);
    // };
    addAndMakeVisible (mSlider);

    // mLabel.setFont (juce::Font (12.0f, juce::Font::plain));
    mLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    mLabel.setFont (11.0f);
    mLabel.setText (juce::String (labelText).toUpperCase(), juce::dontSendNotification);
    mLabel.setJustificationType (juce::Justification::centred);

    // mLabel.attachToComponent (&mSlider, true);
    addAndMakeVisible (mLabel);
}

Dial::~Dial()
{
    mSlider.setLookAndFeel (nullptr);
    mLabel.setLookAndFeel (nullptr);
    setLookAndFeel (nullptr);
}

void Dial::resized()
{
    auto const fontSize = calculatePrimaryTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
    mLabel.setFont (fontSize);

    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (1)), Track (Fr (4)) };
    grid.templateColumns = { Track (Fr (1)) };
    grid.items = { Item (mLabel), Item (mSlider) };
    grid.performLayout (getLocalBounds());
}
} // namespace gui
} // namespace pe