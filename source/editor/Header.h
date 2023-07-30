#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "processor/LevelMeter.h"

namespace pe {
namespace gui {

class Header : public juce::Component {
   public:
    Header(std::shared_ptr<juce::AudioProcessorValueTreeState> parameters);

    void resized() override;

   private:
    juce::ImageComponent mLogo;
    juce::ToggleButton mBypass;
    juce::AudioProcessorValueTreeState::ButtonAttachment mAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Header)
};
}  // namespace gui
}  // namespace pe