#pragma once

#include "DialLookAndFeel.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
class Dial : public juce::Component
{
public:
    Dial (std::string const& labelText,
          std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
          std::string const& parameterId);
    ~Dial() override;

    void resized() override;

protected:
    DialLookAndFeel mLookAndFeel;
    juce::Slider mSlider;
    juce::Label mLabel;
    juce::AudioProcessorValueTreeState::SliderAttachment mAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dial)
};
} // namespace gui
} // namespace pe