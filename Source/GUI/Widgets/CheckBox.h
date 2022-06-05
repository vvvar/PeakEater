#pragma once
#include <JuceHeader.h>

#include "../AppColours.h"

namespace pe
{
namespace gui
{
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
        ~CheckBoxLookAndFeel() override {}

        void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
        {
            auto fontSize = juce::jmin (10.0f, (float) button.getHeight() * 0.75f);
            auto tickWidth = fontSize * 1.1f;

            drawTickBox (g, button, 4.0f, ((float) button.getHeight() - tickWidth) * 0.5f, tickWidth, tickWidth, button.getToggleState(), button.isEnabled(), shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

            g.setColour (button.findColour (juce::ToggleButton::textColourId));
            g.setFont (fontSize);

            if (! button.isEnabled())
                g.setOpacity (0.5f);

            g.drawFittedText (button.getButtonText(),
                              button.getLocalBounds().withTrimmedLeft (juce::roundToInt (tickWidth) + 10).withTrimmedRight (2),
                              juce::Justification::centredLeft,
                              10);
        }

        void drawTickBox (juce::Graphics& g, juce::Component& component, float x, float y, float w, float h, const bool ticked, const bool isEnabled, const bool shouldDrawButtonAsHighlighted, const bool shouldDrawButtonAsDown) override
        {
            juce::ignoreUnused (isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

            juce::Rectangle<float> tickBounds (x, y, w, h);

            g.setColour (component.findColour (juce::ToggleButton::tickDisabledColourId));
            g.drawRoundedRectangle (tickBounds, 4.0f, 1.0f);

            if (ticked)
            {
                g.setColour (component.findColour (juce::ToggleButton::tickColourId));
                auto tick = getTickShape (0.75f);
                g.fillPath (tick, tick.getTransformToScaleToFit (tickBounds.reduced (1, 2).toFloat(), false));
            }
        }
    };

    //==============================================================================
    CheckBox (
        params::Parameter const& parameter,
        juce::AudioProcessorValueTreeState& vts,
        juce::String labelText = "",
        juce::String tooltipText = "") noexcept
        : toggle (parameter.getId())
    {
        juce::String text = labelText.isNotEmpty() ? labelText : parameter.getLabel();
        toggle.setButtonText (text.toUpperCase());
        toggle.setLookAndFeel (&lnf);
        toggle.setTooltip (tooltipText);
        attachment.reset (new ButtonAttachment (vts, parameter.getId(), toggle));

        addAndMakeVisible (toggle);
    }
    ~CheckBox() override
    {
        toggle.setLookAndFeel (nullptr);
    }

    //==============================================================================
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
        grid.templateColumns = { Track (Fr (1)) };

        grid.items = {
            Item (toggle)
        };

        grid.performLayout (getLocalBounds());
    }

    //==============================================================================
    void addListener (juce::Button::Listener* listener)
    {
        toggle.addListener (listener);
    }

    //==============================================================================
    void setEnabled (bool isEnabled)
    {
        toggle.setEnabled (isEnabled);
        if (! isEnabled && toggle.getToggleState()) // if we're disabling checkbox that is checked
        {
            toggle.setToggleState (false, juce::sendNotificationSync); // uncheck it
        }
    }

private:
    //==============================================================================
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    //==============================================================================
    juce::ToggleButton toggle;
    std::unique_ptr<ButtonAttachment> attachment;
    CheckBoxLookAndFeel lnf;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CheckBox)
};
} // namespace widgets
} // namespace gui
} // namespace pe
