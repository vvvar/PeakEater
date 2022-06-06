#pragma once
#include <JuceHeader.h>

#include "../AppColours.h"

namespace pe
{
namespace gui
{
namespace widgets
{
// ==============================================================================
class LabelListener
{
public:
    virtual ~LabelListener() = default;

    virtual void onClick (const juce::MouseEvent&) = 0;
};

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
        ~LabelLookAndFeel() override {}
        juce::Font getLabelFont (juce::Label& label) override
        {
            label.setJustificationType (juce::Justification::centred);
            return { 10.0f };
        }
    };

    //==============================================================================
    Label (juce::String const& componentName = {}, juce::String const& labelText = {})
        : juce::Label (componentName, labelText)
    {
        setLookAndFeel (&lnf);
    }

    ~Label()
    {
        setLookAndFeel (nullptr);
    }

    void onClick (LabelListener* listener)
    {
        listeners.add (listener);
    }

    void mouseDown (const juce::MouseEvent& event)
    {
        listeners.call ([&event] (LabelListener& listener)
                        { listener.onClick (event); });
    }

private:
    //==============================================================================
    LabelLookAndFeel lnf;

    //==============================================================================
    juce::ListenerList<LabelListener> listeners;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Label)
};
} // namespace widgets
} // namespace gui
} // namespace pe
