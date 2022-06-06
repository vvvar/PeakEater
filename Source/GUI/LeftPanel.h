#pragma once

#include <JuceHeader.h>

#include "Widgets/LabledMeter.h"

namespace pe
{
namespace gui
{
namespace layout
{
//==============================================================================
class LeftPanel : public juce::Component
{
public:
    //==============================================================================
    LeftPanel (PeakEaterAudioProcessor& p) : inputMeter ("IN", p.getInputMeterSource())
    {
        addAndMakeVisible (inputMeter);
    }

    //==============================================================================
    void paint (juce::Graphics&) override
    {
    }

    void resized() override
    {
        using Grid = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr = Grid::Fr;
        using Item = juce::GridItem;

        Grid grid;

        grid.templateRows = { Track (Fr (1)) };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = { Item (inputMeter) };

        grid.performLayout (getLocalBounds());
    }

private:
    //==============================================================================
    widgets::LabledMeter inputMeter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftPanel)
};
} // namespace layout
} // namespace gui
} // namespace pe
