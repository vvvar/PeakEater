#include "LeftPanel.h"

namespace pe
{
namespace gui
{
    LeftPanel::LeftPanel (LevelMetersPack const& levelMetersPack, std::shared_ptr<Ticks> ticks)
        : juce::Component(), mLevelMeter (levelMetersPack.inputLevelMeter, ticks, "IN")
    {
        addAndMakeVisible (mLevelMeter);
    }

    void LeftPanel::resized() { mLevelMeter.setBounds (getLocalBounds()); }

    void LeftPanel::paint (juce::Graphics& g) { g.fillAll (juce::Colour (22, 33, 62)); }
} // namespace gui
} // namespace pe