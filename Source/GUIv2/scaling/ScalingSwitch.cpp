#include "ScalingSwitch.h"

#include "../ColourScheme.h"
#include "../Utils.h"

namespace pe
{
namespace gui
{
ScalingSwitch::ScalingSwitch (std::shared_ptr<Ticks> ticks)
	: mTicks (ticks)
{
	setMouseCursor (juce::MouseCursor::PointingHandCursor);
}

ScalingSwitch::~ScalingSwitch()
{
	setLookAndFeel (nullptr);
}

juce::String ScalingSwitch::getTooltip()
{
	return "Scaling";
}

void ScalingSwitch::paint (juce::Graphics& g)
{
	juce::Colour color = colourscheme::BackgroundTertiary.withAlpha (0.8f);
	if (isMouseOver())
	{
		color = color.withAlpha (1.0f);
	}
	g.setColour (color);
	g.drawText (mTicks->isLinear() ? "1x" : "2x", getLocalBounds(), juce::RectanglePlacement::centred, true);
}

void ScalingSwitch::mouseDown (juce::MouseEvent const&)
{
	mTicks->setIsLinear(!mTicks->isLinear());
}

void ScalingSwitch::mouseEnter (juce::MouseEvent const&)
{
	if (isEnabled())
	{
		setMouseCursor (juce::MouseCursor::PointingHandCursor);
	}
	else
	{
		setMouseCursor (juce::MouseCursor::NormalCursor);
	}
}

void ScalingSwitch::mouseExit (juce::MouseEvent const&)
{
	if (isEnabled())
	{
		setMouseCursor (juce::MouseCursor::PointingHandCursor);
	}
	else
	{
		setMouseCursor (juce::MouseCursor::NormalCursor);
	}
}
} // namespace gui
} // namespace pe