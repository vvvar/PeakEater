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
class Master : public widgets::BlockPanel
{
public:
    Master (juce::AudioProcessorValueTreeState& vts):
    widgets::BlockPanel ("MASTER"),
    input (Parameters::InputGain, vts),
    linkInOut (Parameters::LinkInOut, vts),
    output (Parameters::OutputGain, vts)
    {
        addAndMakeVisible (input);
        addAndMakeVisible (linkInOut);
        addAndMakeVisible (output);
    }
    
    //==============================================================================
    void resized() override
    {
        widgets::BlockPanel::resized();
        
        using Grid  = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr    = Grid::Fr;
        using Item  = juce::GridItem;
        
        Grid grid;

        grid.templateRows = {
            Track (Fr (1)),
            Track (Fr (1)),
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (2)), Track (Fr (1)), Track (Fr (2)) };
        grid.items = {
            Item(input).withMargin (Item::Margin (0, 0, 0, 5)), Item(linkInOut).withMargin (Item::Margin (0, 5, 10, 5)), Item(output).withMargin (Item::Margin (0, 5, 0, 0))
        };
         
        grid.performLayout (getReducedBounds().toNearestInt());
    }
private:
    widgets::LabledRotarySlider input;
    LinkInOutButton linkInOut;
    widgets::LabledRotarySlider output;
};

}

