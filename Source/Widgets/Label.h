#pragma once
#include <JuceHeader.h>

#include "../AppColours.h"

namespace widgets
{

//==============================================================================
class Label : public juce::Label
{
public:
    //==============================================================================
    class LabelLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        LabelLookAndFeel()
        {
            setColour (juce::Label::textColourId, AppColors::Navy);
        }
        ~LabelLookAndFeel() {}
        juce::Font getLabelFont (juce::Label& label) override
        {
            label.setJustificationType (juce::Justification::centred);
            return { 10.0f };
        }
    };
    
    //==============================================================================
    Label(const juce::String &componentName = {}, const juce::String &labelText = {})
    : juce::Label(componentName, labelText)
    {
        setLookAndFeel(&lnf);
    }
    
    ~Label()
    {
        setLookAndFeel(nullptr);
    }
    
private:
    //==============================================================================
    LabelLookAndFeel lnf;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Label)
};

}
