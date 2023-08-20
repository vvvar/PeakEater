#include "RightPanel.h"

namespace pe::gui
{

RightPanel::RightPanel (LevelMetersPack const& levelMetersPack, std::shared_ptr<Ticks> ticks)
    : juce::Component(), mLevelMeter (levelMetersPack.outputLevelMeter, ticks, "OUT")
{
    addAndMakeVisible (mLevelMeter);
}

void RightPanel::resized() { mLevelMeter.setBounds (getLocalBounds()); }

} // namespace pe::gui
