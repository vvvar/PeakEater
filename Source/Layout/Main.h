#pragma once

#include <JuceHeader.h>
#include "LeftPanel.h"
#include "RightPanel.h"
#include "CentralPanel.h"
#include "../AppTheme.h"

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
        auto typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::MontserratRegular_ttf, BinaryData::MontserratRegular_ttfSize);
        juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(typeface);
        juce::LookAndFeel::getDefaultLookAndFeel().setColour (juce::Label::textColourId, AppTheme::V1::TextColor);

        // juce::LookAndFeel::getDefaultLookAndFeel().setColour (juce::ToggleButton::tickColourId, AppColors::Navy);
        
        tooltipWindow.setColour(juce::TooltipWindow::backgroundColourId, AppTheme::V1::TooltipBackgroundColor);
        tooltipWindow.setColour(juce::TooltipWindow::textColourId, AppTheme::V1::TooltipTextColor);
        tooltipWindow.setColour(juce::TooltipWindow::outlineColourId, AppTheme::V1::TooltipOutlineColor);
        
        addAndMakeVisible(leftPanel);
        addAndMakeVisible(centralPanel);
        addAndMakeVisible(rightPanel);
    }
    
    void paint (juce::Graphics& g) override
    {
        // g.fillAll (AppTheme::V1::BackgroundColor);
        
        auto bounds = getLocalBounds().toFloat();
        auto horizontalGradient = juce::ColourGradient (AppTheme::V1::BackgroundGradientFrom,
                                                        bounds.getX(), bounds.getBottom(),
                                                        AppTheme::V1::BackgroundGradientTo,
                                                        bounds.getX(), bounds.getY(), false);
//        horizontalGradient.addColour (0.5, AppTheme::V1::BackgroundGradientFrom);
//        horizontalGradient.addColour (0.75, AppTheme::V1::BackgroundGradientTo);
        g.setGradientFill (horizontalGradient);
        g.fillRect (bounds);
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
