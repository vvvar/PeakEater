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
	: mDialValue (parameters, parameterId)
	, mSliderAttachment (*parameters, parameterId, mSlider)
{
	mSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
	mSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
	mSlider.setLookAndFeel (&mLookAndFeel);
	mSlider.setVelocityModeParameters (1.0, 1, 0.02, true, juce::ModifierKeys::shiftModifier);
	mSlider.setTextBoxIsEditable (false);
	addAndMakeVisible (mSlider);
	addAndMakeVisible (mDialValue);
	mLabel.setFont (11.0f);
	mLabel.setText (juce::String (labelText).toUpperCase(), juce::dontSendNotification);
	mLabel.setJustificationType (juce::Justification::centred);
	addAndMakeVisible (mLabel);
	updateFontSize();
}

Dial::~Dial()
{
	mSlider.setLookAndFeel (nullptr);
	mLabel.setLookAndFeel (nullptr);
	setLookAndFeel (nullptr);
	mSlider.removeMouseListener (this);
}

void Dial::resized()
{
	juce::Grid grid;
	using Track = juce::Grid::TrackInfo;
	using Fr = juce::Grid::Fr;
	using Item = juce::GridItem;
	grid.templateRows = { Track (Fr (2)), Track (Fr (5)), Track (Fr (1)) };
	grid.templateColumns = { Track (Fr (1)) };
	grid.items = { Item (mLabel), Item (mSlider), Item (mDialValue) };
	grid.performLayout (getLocalBounds());
	updateFontSize();
}

void Dial::updateFontSize()
{
	auto const topLevelBounds = getTopLevelComponent()->getBounds();
	auto const topLevelWidth = topLevelBounds.getWidth();
	auto const topLevelHeight = topLevelBounds.getHeight();
	mLabel.setFont (calculatePrimaryTextSize (topLevelWidth, topLevelHeight));
}
} // namespace gui
} // namespace pe