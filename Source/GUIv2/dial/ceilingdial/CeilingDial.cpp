#include "CeilingDial.h"

namespace pe
{
namespace gui
{
CeilingDial::CeilingDial (std::string const& labelText,
                          std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                          std::string const& parameterId,
                          std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                          std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter)
    : Dial::Dial (labelText, parameters, parameterId)
    , mCeilingDialLookAndFeel (inputLevelMeter, clippingLevelMeter)
    , mTimer (std::bind (&CeilingDial::onTimerTick, this))
{
    mSlider.setLookAndFeel (&mCeilingDialLookAndFeel);
    mSlider.setTextValueSuffix (" dB");
    mTimer.startTimer (20);
}

CeilingDial::~CeilingDial()
{
    mSlider.setLookAndFeel (nullptr);
    setLookAndFeel (nullptr);
}

void CeilingDial::onTimerTick()
{
    repaint();
}
} // namespace gui
} // namespace pe