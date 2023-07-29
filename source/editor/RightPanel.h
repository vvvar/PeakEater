#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "LevelMetersPack.h"
#include "Ticks.h"
#include "levelmeter/LevelMeterComponent.h"

namespace pe::gui
{

class RightPanel : public juce::Component
{
public:
    RightPanel (LevelMetersPack const& levelMetersPack, std::shared_ptr<Ticks> ticks);

    void resized() override;

private:
    LevelMeterComponent mLevelMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightPanel)
};

} // namespace pe::gui
