#pragma once

#include <JuceHeader.h>

#include "Label.h"
#include "RotarySlider.h"

namespace widgets
{

//==============================================================================
class LabledRotarySlider : public juce::Component
{
public:
    //==============================================================================
    LabledRotarySlider(
                       const Parameters::ParameterInfo& parameter,
                       juce::AudioProcessorValueTreeState& vts,
                       juce::String tooltipText = ""):
    slider (parameter, vts, tooltipText),
    label (parameter.Id + "Label", parameter.Label)
    {
        addAndMakeVisible (slider);
        addAndMakeVisible (label);
    }
    
    void resized() override
    {
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.alignItems = juce::FlexBox::AlignItems::flexStart;

        fb.items.add (juce::FlexItem (slider).withMinWidth (50.0f).withMinHeight (50.0f));
        fb.items.add (juce::FlexItem (label).withMinWidth (50.0f).withMinHeight (20.0f));

        fb.performLayout (getLocalBounds().toFloat());
    }
private:
    //==============================================================================
    widgets::RotarySlider slider;
    widgets::Label        label;
};

}
