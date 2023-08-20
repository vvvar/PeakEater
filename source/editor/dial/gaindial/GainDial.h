#pragma once

#include "../Dial.h"
#include "GainDialLookAndFeel.h"

namespace pe
{
namespace gui
{
    class GainDial : public Dial
    {
    public:
        GainDial (std::string const& labelText,
                  std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                  std::string const& parameterId);
        ~GainDial() override;

    private:
        GainDialLookAndFeel mGainDialLookAndFeel;
        std::shared_ptr<juce::AudioProcessorValueTreeState> mParameters;
    };
} // namespace gui
} // namespace pe