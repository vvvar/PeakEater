#pragma once

#include <atomic>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
    class BypassButton
        : public juce::Component,
          public juce::AudioProcessorParameter::Listener,
          public juce::TooltipClient
    {
    public:
        BypassButton (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters);
        ~BypassButton() override;

        /** juce::Component */
        void resized() override;
        void paint (juce::Graphics& g) override;
        void mouseDown (juce::MouseEvent const& event) override;
        void drawBypassButton (juce::Graphics& g);

        /** juce::AudioProcessorParameter::Listener */
        void parameterValueChanged (int parameterIndex, float newValue) override;
        void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;

        /** juce::TooltipClient */
        juce::String getTooltip() override;

    private:
        std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
        std::atomic<bool> mIsOn;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BypassButton)
    };
} // namespace gui
} // namespace pe