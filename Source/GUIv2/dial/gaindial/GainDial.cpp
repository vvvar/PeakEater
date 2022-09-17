#include "GainDial.h"

namespace pe
{
namespace gui
{
GainDial::GainDial (std::string const& labelText,
                    std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                    std::string const& parameterId)
    : Dial::Dial (labelText, parameters, parameterId)
    , mParameters (parameters)
{
    mSlider.setLookAndFeel (&mGainDialLookAndFeel);
    mSlider.setTextValueSuffix (" dB");
}

GainDial::~GainDial()
{
    mSlider.setLookAndFeel (nullptr);
    setLookAndFeel (nullptr);
}
} // namespace gui
} // namespace pe