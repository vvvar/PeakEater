#pragma once

#include <JuceHeader.h>

#include "Widgets/LabledMeterSlider.h"
#include "Widgets/LabledSlider.h"

namespace pe
{
namespace gui
{
namespace layout
{
//==============================================================================
class WorkingArea : public juce::Component
{
public:
    //==============================================================================
    WorkingArea (PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
        : inputGain (juce::Slider::SliderStyle::LinearVertical,
                     juce::Slider::TextEntryBoxPosition::TextBoxBelow,
                     params::ParametersProvider::getInstance().getInputGain(),
                     vts,
                     "Adjust gain level applied to the signal before it's processed.")
        , outputGain (juce::Slider::SliderStyle::LinearVertical,
                      juce::Slider::TextEntryBoxPosition::TextBoxBelow,
                      params::ParametersProvider::getInstance().getOutputGain(),
                      vts,
                      "Adjust gain level applied to the signal after it was processed.")
        , ceiling (p.getCeilingMeterSource(),
                   vts,
                   params::ParametersProvider::getInstance().getCeiling(),
                   "Clips everything which is above this threshold.")
        , clippingType (juce::Slider::SliderStyle::LinearVertical,
                        juce::Slider::TextEntryBoxPosition::TextBoxBelow,
                        params::ParametersProvider::getInstance().getClippingType(),
                        vts,
                        "Type of clipping. The hard clip is harshest but less coloring, arctangent is smoothest but adds more color.")
        , oversampleRate (juce::Slider::SliderStyle::LinearVertical,
                          juce::Slider::TextEntryBoxPosition::TextBoxBelow,
                          params::ParametersProvider::getInstance().getOversampleRate(),
                          vts,
                          "High values will suppress aliasing better and increases CPU consumption.")
    {
        addAndMakeVisible (inputGain);
        addAndMakeVisible (outputGain);
        addAndMakeVisible (ceiling);
        addAndMakeVisible (clippingType);
        addAndMakeVisible (oversampleRate);
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

        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)), Track (Fr (1)) };
        grid.items = {
            Item (inputGain),
            Item (clippingType),
            Item (ceiling),
            Item (oversampleRate),
            Item (outputGain),
        };

        grid.performLayout (getLocalBounds());
    }

    //==============================================================================
    void setOutputGainEnabled (bool isEnabled)
    {
        outputGain.setEnabled (isEnabled);
    }

    void setEnabled (bool isEnabled)
    {
        inputGain.setEnabled (isEnabled);
        outputGain.setEnabled (isEnabled);
        ceiling.setEnabled (isEnabled);
        clippingType.setEnabled (isEnabled);
        oversampleRate.setEnabled (isEnabled);
    }

private:
    //==============================================================================
    widgets::LabledSlider inputGain;
    widgets::LabledSlider outputGain;
    widgets::LabledMeterSlider ceiling;
    widgets::LabledSlider clippingType;
    widgets::LabledSlider oversampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WorkingArea)
};
} // namespace layout
} // namespace gui
} // namespace pe