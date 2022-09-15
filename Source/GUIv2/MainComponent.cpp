#include "MainComponent.h"

namespace pe
{
namespace gui
{
namespace
{
auto constexpr gBypassParamName = "Bypass";
}
MainComponent::MainComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                              std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                              std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                              std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
    : juce::Component()
    , mHeader (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
    , mWorkingPanel (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter)
    , mParameters (parameters)
{
    mParameters->getParameter (gBypassParamName)->addListener (this);
    //addAndMakeVisible (mHeader);
    addAndMakeVisible (mWorkingPanel);
}

MainComponent::~MainComponent()
{
    mParameters->getParameter (gBypassParamName)->removeListener (this);
    setLookAndFeel (nullptr);
}

void MainComponent::resized()
{
    juce::Grid grid;
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Item = juce::GridItem;
    grid.templateRows = { Track (Fr (13)) };
    grid.templateColumns = { Track (Fr (1)) };
    grid.items = { Item (mWorkingPanel) };
    grid.performLayout (getLocalBounds());
}

void MainComponent::paint (juce::Graphics& g)
{
    auto darkBlue = juce::Colour (22, 33, 62);
    g.fillAll (darkBlue);
}

void MainComponent::parameterValueChanged (int parameterIndex, float newValue)
{
    mWorkingPanel.setEnabled (! mParameters->getParameter (gBypassParamName)->getValue());
    mWorkingPanel.repaint();
}

void MainComponent::parameterGestureChanged (int parameterIndex, bool gestureIsStarting)
{
    mWorkingPanel.setEnabled (! mParameters->getParameter (gBypassParamName)->getValue());
    mWorkingPanel.repaint();
}
} // namespace gui
} // namespace pe