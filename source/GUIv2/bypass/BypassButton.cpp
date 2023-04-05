#include "BypassButton.h"

#include "../ColourScheme.h"
#include "../Utils.h"

#include <BinaryData.h>

namespace pe
{
namespace gui
{
namespace
{
auto constexpr gParamName = "Bypass";
}
BypassButton::BypassButton (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters)
	: mParameters (parameters)
	, mIsOn (getBoolParamValue(gParamName, parameters))
{
	mParameters->getParameter (gParamName)->addListener (this);
	setMouseCursor (juce::MouseCursor::PointingHandCursor);
}

BypassButton::~BypassButton()
{
	mParameters->getParameter (gParamName)->removeListener (this);
	setLookAndFeel (nullptr);
}

void BypassButton::resized()
{
	// mToggle.setBounds (getLocalBounds());
}

void BypassButton::paint (juce::Graphics& g)
{
	drawBypassButton (g);
}

void BypassButton::drawBypassButton (juce::Graphics& g)
{
	// Set color for lines and icon
	juce::Colour iconColor;
	if (mIsOn)
	{
		iconColor = colourscheme::ForegroundTertiary.withAlpha (0.8f);
	}
	else
	{
		iconColor = colourscheme::BackgroundTertiary.withAlpha (0.8f);
	}
	if (isMouseOver())
	{
		iconColor = iconColor.withAlpha (1.0f);
	}

	auto const bounds = getLocalBounds().toFloat().reduced (1.0f);

	// Draw Border
	g.setColour (iconColor);
	auto const borderRadius = 50.0f;
	auto const borderThickness = 1.0f;
	g.drawRoundedRectangle (bounds, borderRadius, borderThickness);
	g.setColour (iconColor.withAlpha (0.4f));
	g.fillRoundedRectangle (bounds, borderRadius);

	// Draw Icon
	auto const icon = juce::ImageCache::getFromMemory (BinaryData::bypass_png, BinaryData::bypass_pngSize);
	g.setColour (iconColor);
	g.drawImage (icon, bounds, juce::RectanglePlacement::centred, true);
}

void BypassButton::mouseDown (juce::MouseEvent const&)
{
	mParameters->getParameter (gParamName)->setValueNotifyingHost (!mIsOn);
}

void BypassButton::parameterValueChanged (int, float)
{
	mIsOn = getBoolParamValue(gParamName, mParameters);
	repaint();
}

void BypassButton::parameterGestureChanged (int, bool)
{
	mIsOn = getBoolParamValue(gParamName, mParameters);
	repaint();
}

juce::String BypassButton::getTooltip()
{
	if (mIsOn)
	{
		return "Turn Off Bypass";
	}
	else
	{
		return "Turn On Bypass";
	}
}
} // namespace gui
} // namespace pe