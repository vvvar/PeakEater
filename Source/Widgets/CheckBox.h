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
            setColour (juce::ToggleButton::textColourId, AppColors::Navy);
            setColour (juce::ToggleButton::tickColourId, AppColors::Navy);
            setColour (juce::ToggleButton::tickDisabledColourId, AppColors::Blue);
        }
        ~CheckBoxLookAndFeel() {}
        
//        juce::Font getTextButtonFont (juce::TextButton&, int) override
//        {
//            return { 1.0f };
//        }
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
        toggle.setButtonText(text.toUpperCase());
        toggle.setLookAndFeel(&lnf);
        attachment.reset (new ButtonAttachment (vts, parameter.Id, toggle));
        
        addAndMakeVisible (toggle);
    }
    ~CheckBox()
    {
        toggle.setLookAndFeel(nullptr);
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
        grid.templateColumns = { Track (Fr (1)) };

        grid.items = {
            Item (toggle)
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
        toggle.setEnabled (isEnabled);
    }
    
private:
    //==============================================================================
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    //==============================================================================
    juce::ToggleButton                toggle;
    std::unique_ptr<ButtonAttachment> attachment;
    CheckBoxLookAndFeel               lnf;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CheckBox)
};

}
