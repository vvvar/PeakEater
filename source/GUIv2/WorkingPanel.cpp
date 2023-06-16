#include "WorkingPanel.h"

namespace pe
{
namespace gui
{
    WorkingPanel::WorkingPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                                std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                                std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                                std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
        : juce::Component(), mTicks (std::make_shared<Ticks>()), mLeftPanel (inputLevelMeter, mTicks), mCentralPanel (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter, mTicks), mRightPanel (parameters, inputLevelMeter, clippingLevelMeter, outputLevelMeter, mTicks)
    {
        addAndMakeVisible (mLeftPanel);
        addAndMakeVisible (mCentralPanel);
        addAndMakeVisible (mRightPanel);
    }

    void WorkingPanel::resized()
    {
        juce::Grid grid;
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        grid.templateRows = { Track (Fr (1)) };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (12)), Track (Fr (1)) };
        grid.items = { Item (mLeftPanel), Item (mCentralPanel), Item (mRightPanel) };
        grid.performLayout (getLocalBounds());
    }
} // namespace gui
} // namespace pe