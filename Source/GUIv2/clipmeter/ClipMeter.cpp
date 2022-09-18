#include "ClipMeter.h"

#include <BinaryData.h>
#include <cmath>
#include <juce_graphics/juce_graphics.h>
#include <limits>
#include <stdint.h>

#include "../../Parameters.h"
#include "../Utils.h"

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
ClipMeter::ClipMeter (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                      std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter,
                      std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter,
                      std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter)
    : juce::Component()
    , mTimer (std::bind (&ClipMeter::onTimerTick, this))
    , mParameters (parameters)
    , mInputLevelMeter (inputLevelMeter)
    , mClippingLevelMeter (clippingLevelMeter)
    , mOutputLevelMeter (outputLevelMeter)
    , mBufferMaxSize (400)
{
    for (int x = 0; x < mBufferMaxSize; x++)
    {
        mInputBuffer.push_back (-std::numeric_limits<float>::infinity());
        mClippingBuffer.push_back (-std::numeric_limits<float>::infinity());
        mOutputBuffer.push_back (-std::numeric_limits<float>::infinity());
    }
    mTimer.startTimer (30);
}

ClipMeter::~ClipMeter()
{
    setLookAndFeel (nullptr);
}

void ClipMeter::paint (juce::Graphics& g)
{
    mInputBuffer.pop_front();
    mInputBuffer.push_back (mInputLevelMeter->getDecibels());

    mClippingBuffer.pop_front();
    mClippingBuffer.push_back (mClippingLevelMeter->getDecibels());

    auto darkBlue = juce::Colour (22, 33, 62);
    auto lightBlue = juce::Colour (15, 52, 96);
    auto red = juce::Colour (233, 69, 96);
    auto white = juce::Colours::white.withAlpha (0.9f);

    g.fillAll (darkBlue);

    drawTicks (mTicks.getTicksList(), lightBlue, g);
    drawBuffer (mInputBuffer, red.withAlpha (0.5f), g);
    drawBuffer (mClippingBuffer, darkBlue.withAlpha (0.5f), g);
    drawDbLine (*static_cast<juce::AudioParameterFloat*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getCeiling().getId())), white, g);
    drawTicksTexts (mTicks.getTicksList(), red, g);
}

void ClipMeter::drawBuffer (std::deque<float>& buffer, juce::Colour&& colour, juce::Graphics& g)
{
    drawBuffer (buffer, colour, g);
}

void ClipMeter::drawBuffer (std::deque<float>& buffer, juce::Colour& colour, juce::Graphics& g)
{
    auto const bounds = getBounds();
    auto const width = static_cast<float> (bounds.getWidth());
    auto const height = static_cast<float> (bounds.getHeight());

    juce::Path p;

    g.setColour (colour);
    float offset = 0.0f;
    float offsetCoef = width / static_cast<float> (mBufferMaxSize);
    juce::Point<float> currentPoint{ offset, height };
    p.startNewSubPath (currentPoint);
    for (auto& db : buffer)
    {
        juce::Point<float> nextPoint (offset, gDbToYPos (db, height));
        p.lineTo (nextPoint);
        currentPoint = nextPoint;
        offset = offset + offsetCoef;
    }
    juce::Point<float> lastPoint (width, height);
    p.lineTo (lastPoint);
    p.closeSubPath();
    g.fillPath (p);
}

void ClipMeter::drawDbLine (float const& dB, juce::Colour& colour, juce::Graphics& g)
{
    auto const bounds = getBounds();
    auto const width = static_cast<float> (bounds.getWidth());
    auto const height = static_cast<float> (bounds.getHeight());
    auto const yPos = gDbToYPos (dB, height);
    juce::Point<float> start (0.0f, yPos);
    juce::Point<float> end (width, yPos);
    juce::Line<float> line (start, end);
    g.setColour (colour);
    g.drawLine (line, 0.5f);
}

void ClipMeter::drawTicks (std::vector<float> const& ticksLevels, juce::Colour&& colour, juce::Graphics& g)
{
    drawTicks (ticksLevels, colour, g);
}

void ClipMeter::drawTicks (std::vector<float> const& ticksLevels, juce::Colour& colour, juce::Graphics& g)
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

void ClipMeter::drawTicksTexts (std::vector<float> const& ticksLevels, juce::Colour& colour, juce::Graphics& g)
{
    auto const bounds = getBounds();
    auto const height = static_cast<float> (bounds.getHeight());
    auto const tickWidth = static_cast<float> (bounds.getWidth());
    juce::Colour textColor (colour);
    if (! isEnabled())
    {
        textColor = textColor.withAlpha (0.5f);
    }
    for (auto const& tickLevel : ticksLevels)
    {
        auto const yPos = static_cast<int> (gDbToYPos (tickLevel, height)) + 4;
        auto const fontSize = calculateTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
        auto const textWidth = fontSize * 3;
        auto const textHeight = fontSize;
        g.setFont (fontSize);
        g.setColour (textColor);
        std::string const dbStr = std::to_string (static_cast<int> (tickLevel)) + "dB";
        g.drawText (dbStr, 0, yPos, textWidth, textHeight, juce::Justification::left, true);
        g.drawText (dbStr, tickWidth - textWidth, yPos, textWidth, textHeight, juce::Justification::right, true);
    }
}

void ClipMeter::resized()
{
}

void ClipMeter::onTimerTick()
{
    repaint();
}

void ClipMeter::mouseDown (juce::MouseEvent const& event)
{
    if (event.mods.isRightButtonDown())
    {
        mTicks.switchToNextTicksList();
    }
}
} // namespace gui
} // namespace pe