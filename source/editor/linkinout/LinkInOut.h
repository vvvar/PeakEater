#pragma once

#include <atomic>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
    class LinkInOut
        : public juce::Component,
          public juce::AudioProcessorParameter::Listener
    {
    public:
        LinkInOut (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters);
        ~LinkInOut() override;

        void resized() override;
        void paint (juce::Graphics& g) override;
        void mouseDown (juce::MouseEvent const& event) override;
        void mouseEnter (juce::MouseEvent const& event) override;
        void mouseExit (juce::MouseEvent const& event) override;

        void parameterValueChanged (int parameterIndex, float newValue) override;
        void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;

    private:
        std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
        std::atomic<bool> mIsOn;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LinkInOut)
    };
} // namespace gui
} // namespace pe