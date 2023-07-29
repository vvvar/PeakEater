#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "linkinout/LinkInOut.h"
#include "shared/LevelMeter.h"

namespace pe {
namespace gui {

class LinkingPanel : public juce::Component {
   public:
    LinkingPanel(std::shared_ptr<juce::AudioProcessorValueTreeState> parameters);

    void resized() override;

   private:
    LinkInOut mLinkInOut;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LinkingPanel)
};
}  // namespace gui
}  // namespace pe