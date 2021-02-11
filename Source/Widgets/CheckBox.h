#pragma once
#include <JuceHeader.h>

#include "../AppColours.h"

namespace widgets
{

//==============================================================================
class CheckBox : public juce::Component
{
public:
    //==============================================================================
    class CheckBoxLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CheckBoxLookAndFeel()
        {
            setColour (juce::Label::textColourId, AppColors::Navy);
            setColour (juce::ToggleButton::tickColourId, AppColors::Navy);
            setColour (juce::ToggleButton::tickDisabledColourId, AppColors::Blue);
        }
        ~CheckBoxLookAndFeel() {}
        juce::Font getLabelFont (juce::Label& label) override
        {
            label.setJustificationType (juce::Justification::centred);
            return { 10.0f };
        }
    };
    
    //==============================================================================
    CheckBox(
             const Parameters::ParameterInfo& parameter,
             juce::AudioProcessorValueTreeState& vts,
             juce::String labelText = ""
             ) noexcept :
        toggle(parameter.Id)
    {
        juce::String text = labelText.isNotEmpty() ? labelText : parameter.Label;
        label.setText (text.toUpperCase(), juce::dontSendNotification);
        
        setLookAndFeel(&lnf);
        
        addAndMakeVisible (label);
        addAndMakeVisible (toggle);
        
        attachment.reset (new ButtonAttachment (vts, parameter.Id, toggle));
    }
    ~CheckBox()
    {
        setLookAndFeel(nullptr);
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        juce::Grid grid;
         
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        
        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)) };

        grid.items = {
            Item (label), Item (toggle)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    //==============================================================================
    void addListener(juce::Button::Listener* listener)
    {
        toggle.addListener(listener);
    }
    
    //==============================================================================
    void setEnabled(bool isEnabled)
    {
        label.setEnabled (isEnabled);
        toggle.setEnabled (isEnabled);
    }
    
private:
    //==============================================================================
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    //==============================================================================
    juce::Label                       label;
    juce::ToggleButton                toggle;
    std::unique_ptr<ButtonAttachment> attachment;
    CheckBoxLookAndFeel               lnf;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CheckBox)
};

}
