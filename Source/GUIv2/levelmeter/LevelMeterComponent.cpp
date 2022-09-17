#include "LevelMeterComponent.h"

#include "../Utils.h"
#include <juce_graphics/juce_graphics.h>
#include <stdint.h>

namespace pe
{
namespace gui
{
namespace
{
enum class ScalingType : unsigned int
{
    kLinear,
    kLogarithmic
};
float smoothDbValue (float const& dB, juce::LinearSmoothedValue<float>& smoother)
{
    smoother.skip (1);
    if (dB < smoother.getCurrentValue())
    {
        smoother.setTargetValue (dB);
    }
    else
    {
        smoother.setCurrentAndTargetValue (dB);
    }
    return smoother.getCurrentValue();
}
float gRoundDb (float const& dB)
{
    return ((float) ((int) (dB * 10))) / 10;
}
template <typename T>
std::string gToStringWithPrecision (const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision (n);
    out << std::fixed << a_value;
    return out.str();
}
float gDbToYPos (float const& dB, float const& maxY, float const& maxDb = 36.0f, ScalingType const&& scalingType = ScalingType::kLinear)
{
    if (std::isinf (dB))
    {
        return maxY;
    }
    if (dB >= 0.0f)
    {
        return 0.0f;
    }
    if (scalingType == ScalingType::kLinear)
    {
        return maxY * (std::fabs (dB) / maxDb);
    }
    else
    {
        auto const logMax = std::log (maxDb);
        auto const mappedDb = std::log (std::fabs (dB)) / logMax;
        return maxY * mappedDb;
    }
}
} // namespace
LevelMeterComponent::LevelMeterComponent (std::shared_ptr<pe::dsp::LevelMeter<float>> levelMeter, std::string const& name)
    : juce::Component()
    , mName (name)
    , mMeterTimer (std::bind (&LevelMeterComponent::onTimerTick, this))
    , mLevelMeter (levelMeter)
    , mRange (-36.0f, 0.0f, 0.1f, 1.0f)
    , mSmoothedGain (levelMeter->getDecibels())
    , mLabel()
{
    auto const refreshRate = 30;
    mSmoothedGain.reset (refreshRate, 0.5f);
    mMeterTimer.startTimerHz (refreshRate);
}

LevelMeterComponent::~LevelMeterComponent()
{
    setLookAndFeel (nullptr);
}

void LevelMeterComponent::resized()
{
}

void LevelMeterComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (22, 33, 62));

    drawTicks ({ 0.0f, -3.0f, -5.0f, -8.0f, -11.0f, -14.0f, -17.0f, -20.0f, -23.0f, -26.0f, -29.0f, -32.0f, -36.0f }, juce::Colour (15, 52, 96), g);

    auto const bounds = getLocalBounds();
    auto const width = static_cast<float> (bounds.getWidth());
    auto const height = static_cast<float> (bounds.getHeight());

    //mSmoothedGain.skip (1);
    // mSmoothedGain.setTargetValue (mLevelMeter->getDecibels());
    auto const dB = smoothDbValue (mLevelMeter->getDecibels(), mSmoothedGain);
    auto const poxY = gDbToYPos (dB, height);
    auto const poxX = 0.0f;

    // g.setColour (juce::Colour (233, 69, 96));
    const juce::Point<float> gradientPoint1 = { 0.0f, static_cast<float> (bounds.getBottom()) };
    const juce::Point<float> gradientPoint2 = { 0.0f, static_cast<float> (bounds.getY()) };
    const juce::ColourGradient gradient (juce::Colour (22, 33, 62).withAlpha (0.8f), gradientPoint1, juce::Colour (233, 69, 96).withAlpha (0.8f), gradientPoint2, false);
    g.setGradientFill (gradient);
    g.fillRect (poxX, poxY, width, height - poxY);

    juce::Colour dbValueTextColour (juce::Colours::white.withAlpha (0.8f));
    juce::Colour nameTextClour (juce::Colour (233, 69, 96));
    if (! isEnabled())
    {
        dbValueTextColour = dbValueTextColour.withAlpha (0.5f);
        nameTextClour = nameTextClour.withAlpha (0.5f);
    }

    // Draw dB value
    auto const padding = 15.0f;
    auto const valueTextSize = calculateTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
    g.setFont (valueTextSize);
    g.setColour (dbValueTextColour);
    g.drawText (std::to_string (static_cast<int> (dB)) + "dB", 0.0f, padding, width, height, juce::Justification::centredTop, true);
    auto const textSize = calculatePrimaryTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());

    // Draw name
    g.setFont (textSize);
    g.setColour (nameTextClour);
    g.drawText (mName, 0.0f, 0.0f, width, height - padding, juce::Justification::centredBottom, true);
}

void LevelMeterComponent::drawTicks (std::vector<float> const& ticksLevels, juce::Colour&& colour, juce::Graphics& g)
{
    drawTicks (ticksLevels, colour, g);
}

void LevelMeterComponent::drawTicks (std::vector<float> const& ticksLevels, juce::Colour& colour, juce::Graphics& g)
{
    auto const bounds = getBounds();
    auto const height = static_cast<float> (bounds.getHeight());
    auto const tickWidth = static_cast<float> (bounds.getWidth());
    for (auto const& tickLevel : ticksLevels)
    {
        auto const yPos = gDbToYPos (tickLevel, height);
        juce::Point<float> start (0.0f, yPos);
        juce::Point<float> end (tickWidth, yPos);
        juce::Line<float> line (start, end);
        g.setColour (colour);
        g.drawLine (line, 0.5f);
    }
}

void LevelMeterComponent::onTimerTick()
{
    repaint();
}
} // namespace gui
} // namespace pe