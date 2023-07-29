#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "LevelMetersPack.h"
#include "clipmeter/ClipMeter.h"
#include "dial/Dial.h"
#include "dial/ceilingdial/CeilingDial.h"
#include "dial/gaindial/GainDial.h"

namespace pe
{
namespace gui
{

    class ControlPanel : public juce::Component
    {
    public:
        ControlPanel (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                      LevelMetersPack const& levelMetersPack);

        void paint (juce::Graphics& g) override;
        void resized() override;

    private:
        GainDial mDial_1;
        Dial mDial_2;
        CeilingDial mDial_3;
        Dial mDial_4;
        GainDial mDial_5;
        Dial mDial_6;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPanel)
    };
} // namespace gui
} // namespace pe