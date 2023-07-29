#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Header.h"
#include "LevelMetersPack.h"
#include "WorkingPanel.h"
#include "bypass/BypassButton.h"

namespace pe
{
namespace gui
{

    class MainComponent : public juce::Component, public juce::AudioProcessorParameter::Listener
    {
    public:
        MainComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                       LevelMetersPack const& levelMetersPack);
        ~MainComponent() override;

        void resized() override;
        void paint (juce::Graphics& g) override;
        void parameterValueChanged (int parameterIndex, float newValue) override;
        void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;

    private:
        Header mHeader;
        WorkingPanel mWorkingPanel;
        BypassButton mBypassButton; // part of Main Component because when disabled we still need to handle clicks
        juce::TooltipWindow mTooltipWindow { this, 500 };

        std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    };
} // namespace gui
} // namespace pe