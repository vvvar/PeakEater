#pragma once

#include <ff_meters/ff_meters.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{

class MainComponent : public juce::Component
{
public:
    MainComponent (std::shared_ptr<foleys::LevelMeterSource> levelMeterSource);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    foleys::LevelMeter mLevelMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
} // namespace gui
} // namespace pe