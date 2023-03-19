#include "MainComponent.h"
#include "Utils.h"

namespace pe
{
namespace gui
{
namespace
{
auto constexpr gBypassParamName = "Bypass";
}
MainComponent::MainComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                              std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                              std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                              std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter)
	: juce::Component()
	, mHeader (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
	, mWorkingPanel (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
	, mBypassButton (parameters)
	, mParameters (parameters)
{
	addAndMakeVisible (mWorkingPanel);
	addAndMakeVisible (mBypassButton);
	mParameters->getParameter (gBypassParamName)->addListener (this);
	mWorkingPanel.setEnabled (!getBoolParamValue(gBypassParamName, mParameters));
}

MainComponent::~MainComponent()
{
	mParameters->getParameter (gBypassParamName)->removeListener (this);
	setLookAndFeel (nullptr);
}

void MainComponent::resized()
{
	auto const localBounds = getLocalBounds();

	juce::Grid grid;
	using Track = juce::Grid::TrackInfo;
	using Fr = juce::Grid::Fr;
	using Item = juce::GridItem;
	grid.templateRows = { Track (Fr (13)) };
	grid.templateColumns = { Track (Fr (1)) };
	grid.items = { Item (mWorkingPanel) };
	grid.performLayout (localBounds);

	auto const bypassWidth = localBounds.getHeight() * 0.1f;
	auto const bypassHeight = localBounds.getHeight() * 0.1f;
	auto const paddingTop = localBounds.getHeight() * 0.08f;
	auto const paddingRight = localBounds.getHeight() * 0.225f;
	auto const bypassX = localBounds.getWidth() - bypassWidth - paddingRight;
	auto const bypassY = 0.0f + paddingTop;
	mBypassButton.setBounds(
		juce::Rectangle<float>(bypassWidth, bypassHeight).withX(bypassX).withY(bypassY).toNearestInt());
}

void MainComponent::paint (juce::Graphics& g)
{
	auto darkBlue = juce::Colour (22, 33, 62);
	g.fillAll (darkBlue);
}

void MainComponent::parameterValueChanged (int, float)
{
	mWorkingPanel.setEnabled (!getBoolParamValue(gBypassParamName, mParameters));
	mWorkingPanel.repaint();
}

void MainComponent::parameterGestureChanged (int, bool)
{
	mWorkingPanel.setEnabled (!getBoolParamValue(gBypassParamName, mParameters));
	mWorkingPanel.repaint();
}
} // namespace gui
} // namespace pe