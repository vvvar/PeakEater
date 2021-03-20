#pragma once

#include "../AppTheme.h"
#include "Label.h"

namespace widgets
{

//==============================================================================
class BlockPanel : public juce::Component
{
public:
    BlockPanel(const juce::String& name = "Block"):
    label(name + "Label", name, AppTheme::V1::PanelNameTextSize)
    {
        addAndMakeVisible (label);
    }
    
    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        auto bounds = getReducedBounds();

        g.setColour (AppTheme::V1::PanelBackgroundColor);
        g.fillRoundedRectangle (bounds, AppTheme::V1::PanelBorderRadius);

        g.setColour (AppTheme::V1::PanelBorderColor);
        g.drawRoundedRectangle (bounds,
                                AppTheme::V1::PanelBorderRadius,
                                AppTheme::V1::PanelBorderThickness);
    }
    
    void resized() override
    {
        label.setBounds (10, 0, 80, 20);
    }
    
    juce::Rectangle<float> getReducedBounds() const noexcept
    {
        return getLocalBounds().toFloat().reduced (AppTheme::V1::PanelBorderThickness / 2.0f + 10.0f,
                                                   AppTheme::V1::PanelBorderThickness / 2.0f + 10.0f);
    }
    
private:
    //==============================================================================
    widgets::Label label;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BlockPanel)
};





}
