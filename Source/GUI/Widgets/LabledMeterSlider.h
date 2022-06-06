#pragma once

#include <JuceHeader.h>

#include "../AppColours.h"

#include "Label.h"
#include "MeterSlider.h"

namespace pe
{
namespace gui
{
namespace widgets
{

//==============================================================================
class LabledMeterSlider : public juce::Component
{
public:
    //==============================================================================
    LabledMeterSlider (foleys::LevelMeterSource& meterSource,
                       juce::AudioProcessorValueTreeState& valueTreeState,
                       params::Parameter const& parameter,
                       juce::String sliderTooltip = "")
        : label (parameter.getId() + "Label", parameter.getLabel()), slider (meterSource, valueTreeState, parameter, sliderTooltip)
    {
        addAndMakeVisible (label);
        addAndMakeVisible (slider);
    }

    //==============================================================================
    void resized() override
    {
        using Grid = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr = Grid::Fr;
        using Item = juce::GridItem;

        Grid grid;

        grid.templateRows = {
            Track (Fr (1)), Track (Fr (9))
        };
        grid.templateColumns = {
            Track (Fr (1))
        };
        grid.items = { Item (label), Item (slider) };

        grid.performLayout (getLocalBounds());
    }

    void setEnabled (bool isEnabled)
    {
        label.setEnabled (isEnabled);
        slider.setEnabled (isEnabled);
    }

private:
    //==============================================================================
    widgets::Label label;
    widgets::MeterSlider slider;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabledMeterSlider)
};
} // namespace widgets
} // namespace gui
} // namespace pe
