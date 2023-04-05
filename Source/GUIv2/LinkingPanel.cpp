#include "LinkingPanel.h"

#include "ColourScheme.h"

namespace pe
{
namespace gui
{
LinkingPanel::LinkingPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters)
	: juce::Component()
	, mLinkInOut (parameters)
{
	addAndMakeVisible (mLinkInOut);
}

void LinkingPanel::resized()
{
	mLinkInOut.setBounds (getLocalBounds());
}
} // namespace gui
} // namespace pe