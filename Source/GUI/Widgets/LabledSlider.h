#pragma once

#include <JuceHeader.h>

#include "../../Parameters.h"
#include "../AppColours.h"
#include "Label.h"
#include "Slider.h"

namespace pe
{
namespace gui
{
namespace widgets
{

class LabledSlider : public juce::Component
{
public:
    LabledSlider (
        juce::Slider::SliderStyle style,
        juce::Slider::TextEntryBoxPosition textBoxPosition,
        params::Parameter const& parameter,
        juce::AudioProcessorValueTreeState& vts,
        juce::String sliderTooltip = "")
        : label (parameter.getId() + "Label", parameter.getLabel()), slider (style, textBoxPosition, parameter, vts, sliderTooltip)
    {
        addAndMakeVisible (label);
        addAndMakeVisible (slider);
    }

    ~LabledSlider() override
    {
        setLookAndFeel (nullptr);
    }

    void resized() override
    {
        using Grid = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr = Grid::Fr;
        using Item = juce::GridItem;

        Grid grid;

        grid.templateRows = { Track (Fr (1)), Track (Fr (9)) };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = { Item (label), Item (slider) };

        grid.performLayout (getLocalBounds());
    }

    void setEnabled (bool isEnabled)
    {
        label.setEnabled (isEnabled);
        slider.setEnabled (isEnabled);
    }

private:
    widgets::Label label;
    widgets::Slider slider;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabledSlider)
};
} // namespace widgets
} // namespace gui
} // namespace pe
