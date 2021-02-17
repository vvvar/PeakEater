#pragma once

#include <JuceHeader.h>
#include "LeftPanel.h"
#include "RightPanel.h"
#include "CentralPanel.h"
#include "../AppColours.h"

namespace layout
{

//==============================================================================
class MainWindow : public juce::Component
{
public:
    //==============================================================================
    MainWindow(PeakEaterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
        leftPanel(p),
        centralPanel(p, vts),
        rightPanel(p),
        tooltipWindow(this, 1500)
    {
        auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::WalkwayUpperBold_ttf, BinaryData::WalkwayUpperBold_ttfSize);
        juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(typeface);
        juce::LookAndFeel::getDefaultLookAndFeel().setColour (juce::Label::textColourId, AppColors::Navy);

        juce::LookAndFeel::getDefaultLookAndFeel().setColour (juce::ToggleButton::tickColourId, AppColors::Navy);
        
        tooltipWindow.setColour(juce::TooltipWindow::backgroundColourId, AppColors::Navy);
        tooltipWindow.setColour(juce::TooltipWindow::textColourId, AppColors::Paper);
        tooltipWindow.setColour(juce::TooltipWindow::outlineColourId, AppColors::Blue);
        
        addAndMakeVisible(leftPanel);
        addAndMakeVisible(centralPanel);
        addAndMakeVisible(rightPanel);
    }
    
    void paint (juce::Graphics&) override
    {
        
    }
    
    void resized() override
    {
        juce::Grid grid;
         
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        
        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (6)), Track (Fr (1)) };
        
        grid.items = {
            Item (leftPanel), Item (centralPanel), Item (rightPanel)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    //==============================================================================
    layout::LeftPanel    leftPanel;
    layout::CentralPanel centralPanel;
    layout::RightPanel   rightPanel;
    juce::TooltipWindow  tooltipWindow;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

}
