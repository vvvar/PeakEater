#pragma once

#include <JuceHeader.h>
#include "../Parameters.h"
#include "../Widgets/BlockPanel.h"
#include "../Widgets/LabledRotarySlider.h"

namespace layout
{

//==============================================================================
class LinkInOutButton : public juce::ToggleButton
{
public:
    //==============================================================================
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using String           = juce::String;
    using ValueTreeState   = juce::AudioProcessorValueTreeState;
    using ParameterInfo    = Parameters::ParameterInfo;
    
    //==============================================================================
    class LinkInOutLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        //==============================================================================
        using Img = juce::Image;
        using Cache = juce::ImageCache;
        
        //==============================================================================
        LinkInOutLookAndFeel():
        off_normal (Cache::getFromMemory (BinaryData::link_grey_png, BinaryData::link_grey_pngSize)),
        off_over   (Cache::getFromMemory (BinaryData::link_white_png, BinaryData::link_white_pngSize)),
        on_normal  (Cache::getFromMemory (BinaryData::link_green_png, BinaryData::link_green_pngSize)),
        on_over    (Cache::getFromMemory (BinaryData::link_green_2_png, BinaryData::link_green_2_pngSize))
        {}
        
        //==============================================================================
        void drawToggleButton (juce::Graphics &g,
                               juce::ToggleButton &button,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown)
        {
            if (button.getToggleState())
            {
                if (shouldDrawButtonAsHighlighted)
                {
                    drawImageWithinButton (on_over, g, button);
                } else
                {
                    drawImageWithinButton (on_normal, g, button);
                }
            } else
            {
                if (shouldDrawButtonAsHighlighted)
                {
                    drawImageWithinButton (off_over, g, button);
                } else
                {
                    drawImageWithinButton (off_normal, g, button);
                }
            }
        }
    private:
        //==============================================================================
        Img off_normal;
        Img off_over;
        Img on_normal;
        Img on_over;
        
        //==============================================================================
        void drawImageWithinButton (Img &image, juce::Graphics &g, juce::ToggleButton &button)
        {
            g.drawImage (image, g.getClipBounds().toFloat(), juce::RectanglePlacement::centred);
        }
    };
    
    //==============================================================================
    LinkInOutButton(String tooltipText = "") noexcept
    {
        setLookAndFeel (&lnf);
    }
    LinkInOutButton(const ParameterInfo& parameter, ValueTreeState& vts, String tooltipText = "") noexcept
    {
        setLookAndFeel (&lnf);
        attachment.reset (new ButtonAttachment (vts, parameter.Id, *this));
    }
    ~LinkInOutButton()
    {
        setLookAndFeel (nullptr);
    }
    
    //==============================================================================
    void clicked () override
    {
       // setToggleState (!getToggleState(), juce::NotificationType::sendNotification);
    }
    void clicked (const juce::ModifierKeys &modifiers) override
    {
        clicked();
    }
private:
    //==============================================================================
    std::unique_ptr<ButtonAttachment> attachment;
    LinkInOutLookAndFeel lnf;
};

//==============================================================================
class OversampleSwitch : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    using String             = juce::String;
    using ValueTreeState     = juce::AudioProcessorValueTreeState;
    using ParameterInfo      = Parameters::ParameterInfo;
    
    enum RadioButtonIds
    {
        xButtons = 1001
    };
    
    //==============================================================================
    OversampleSwitch (ValueTreeState& vts, String tooltipText = ""):
    valueTreeState (vts)
    {
        x2.setRadioGroupId (xButtons);
        x4.setRadioGroupId (xButtons);
        x8.setRadioGroupId (xButtons);
        x16.setRadioGroupId (xButtons);
        
        auto selectedValue = vts.getRawParameterValue ("OversampleRate");
        setCheckboxState (*selectedValue);
        
        x2.onClick = [this] () { updateToggleState(&x2, "x2"); };
        x4.onClick = [this] () { updateToggleState(&x4, "x4"); };
        x8.onClick = [this] () { updateToggleState(&x8, "x8"); };
        x16.onClick = [this] () { updateToggleState(&x16, "x16"); };
        
        addAndMakeVisible (x2);
        addAndMakeVisible (x4);
        addAndMakeVisible (x8);
        addAndMakeVisible (x16);
        addAndMakeVisible (label);
    }
    
    //==============================================================================
    void updateToggleState (juce::Button* button, juce::String name)
    {
        if (button->getToggleState())
        {
            auto oversampleRate = valueTreeState.getParameterAsValue ("OversampleRate");
            if (name == "x2")
            {
                oversampleRate = 0.0f;
            } else if (name == "x4")
            {
                oversampleRate = 1.0f;
            } else if (name == "x8")
            {
                oversampleRate = 2.0f;
            } else if (name == "x16")
            {
                oversampleRate = 3.0f;
            }
        }
    }
    
    void parameterChanged (const String& parameterID, float newValue) override
    {
        if (parameterID == "OversampleRate")
        {
            setCheckboxState (newValue);
        }
    }
    
    void resized() override
    {
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.alignItems = juce::FlexBox::AlignItems::flexStart;

        fb.items.add (juce::FlexItem (x2).withMinWidth (50.0f).withMinHeight (20.0f));
        fb.items.add (juce::FlexItem (x4).withMinWidth (50.0f).withMinHeight (20.0f));
        fb.items.add (juce::FlexItem (x8).withMinWidth (50.0f).withMinHeight (20.0f));
        fb.items.add (juce::FlexItem (x16).withMinWidth (50.0f).withMinHeight (20.0f));
        fb.items.add (juce::FlexItem (label).withMinWidth (40.0f).withMinHeight (20.0f));

        fb.performLayout (getLocalBounds().toFloat());
    }
    
private:
    //==============================================================================
    juce::ToggleButton x2 { "x2" };
    juce::ToggleButton x4 { "x4" };
    juce::ToggleButton x8 { "x8" };
    juce::ToggleButton x16 { "x16" };
    juce::Label label { "OversampleLabel", "OVERSAMPLE" };
    
    ValueTreeState& valueTreeState;
    
    void setCheckboxState (float selectedValue)
    {
        x2.setToggleState ((selectedValue == 0), juce::NotificationType::dontSendNotification);
        x4.setToggleState ((selectedValue == 1), juce::NotificationType::dontSendNotification);
        x8.setToggleState ((selectedValue == 2), juce::NotificationType::dontSendNotification);
        x16.setToggleState ((selectedValue == 3), juce::NotificationType::dontSendNotification);
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OversampleSwitch)
};

//==============================================================================
class MasterTop : public juce::Component
{
public:
    //==============================================================================
    MasterTop (juce::AudioProcessorValueTreeState& vts):
    input (Parameters::InputGain, vts, " dB"),
    linkInOut (Parameters::LinkInOut, vts, " dB"),
    output (Parameters::OutputGain, vts, " dB")
    {
        addAndMakeVisible (input);
        addAndMakeVisible (linkInOut);
        addAndMakeVisible (output);
    }
    
    //==============================================================================
    void resized() override
    {
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (4)), Track (Fr (1)), Track (Fr (4)) };
        grid.items = {
            Item(input), Item(linkInOut).withMargin(Item::Margin (0, 0, 20, 0)), Item(output)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    //==============================================================================
    widgets::LabledRotarySlider input;
    LinkInOutButton linkInOut;
    widgets::LabledRotarySlider output;
};

//==============================================================================
class MasterBottom : public juce::Component
{
public:
    //==============================================================================
    MasterBottom (juce::AudioProcessorValueTreeState& vts):
    oversample (vts)
    {
        addAndMakeVisible (oversample);
    }
    
    //==============================================================================
    void resized() override
    {
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (2)), Track (Fr (1)) };
        grid.items = {
            Item(oversample)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    //==============================================================================
    OversampleSwitch oversample;
};

//==============================================================================
class Master : public widgets::BlockPanel
{
public:
    Master (juce::AudioProcessorValueTreeState& vts):
    widgets::BlockPanel ("MASTER"),
    masterTop (vts),
    masterBottom (vts)
    {
        addAndMakeVisible (masterTop);
        addAndMakeVisible (masterBottom);
    }
    
    //==============================================================================
    void resized() override
    {
        widgets::BlockPanel::resized();
        
        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.alignItems = juce::FlexBox::AlignItems::flexStart;

        fb.items.add (juce::FlexItem (masterTop).withFlex (1).withMargin (juce::FlexItem::Margin (25, 5, 0, 5)));
        fb.items.add (juce::FlexItem (masterBottom).withFlex (3).withMargin (juce::FlexItem::Margin (10, 18, 0, 18)));

        fb.performLayout (getLocalBounds().toFloat());
    }
private:
    MasterTop    masterTop;
    MasterBottom masterBottom;
};

}

