#include "Header.h"
#include <BinaryData.h>

namespace pe
{
namespace gui
{
Header::Header (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters)
	: juce::Component()
	, mAttachment (*parameters, "Bypass", mBypass)
{
	mLogo.setImage (juce::ImageCache::getFromMemory (BinaryData::logo_plugin_png, BinaryData::logo_plugin_pngSize));
	mLogo.setImagePlacement (juce::RectanglePlacement::xLeft);
	mBypass.setButtonText ("Bypass");
	addAndMakeVisible (mLogo);
	addAndMakeVisible (mBypass);
}

void Header::resized()
{
	juce::Grid grid;
	using Track = juce::Grid::TrackInfo;
	using Fr = juce::Grid::Fr;
	using Item = juce::GridItem;
	grid.templateRows = { Track (Fr (1)) };
	grid.templateColumns = { Track (Fr (4)), Track (Fr (1)) };
	grid.items = { Item (mLogo), Item (mBypass) };
	grid.performLayout (getLocalBounds());
}
} // namespace gui
} // namespace pe