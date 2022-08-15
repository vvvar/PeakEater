#include "ClipMeter.h"

#include <cmath>
#include <juce_graphics/juce_graphics.h>
#include <limits>

#include "../../Parameters.h"

namespace pe
{
namespace gui
{
namespace
{
enum class ScalingType : u_int8_t
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
    , mBufferMaxSize (500)
    //, mTicks({ 0.0f, -5.0f, -8.0f, -11.0f, -14.0f, -17.0f, -20.0f, -23.0f, -26.0f, -29.0f, -32.0f, -36.0f })
    , mTicks ({ 0.0f, -3.0f, -6.0f, -9.0f, -12.0f, -15.0f, -18.0f, -21.0f, -24.0f, -27.0f, -30.0f, -33.0f, -36.0f })
//  , mTicks ({ 0.0f, -3.0f, -6.0f, -9.0f, -12.0f, -18.0f, -36.0f })
{
    for (int x = 0; x < mBufferMaxSize; x++)
    {
        mInputBuffer.push_back (-std::numeric_limits<float>::infinity());
        mClippingBuffer.push_back (-std::numeric_limits<float>::infinity());
        mOutputBuffer.push_back (-std::numeric_limits<float>::infinity());
    }
    mTimer.startTimer (20);
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

    drawBuffer (mInputBuffer, juce::Colour{ juce::Colours::red }, g);
    drawBuffer (mClippingBuffer, juce::Colour{ juce::Colours::blue }, g);
    drawDbLine (*static_cast<juce::AudioParameterFloat*> (mParameters->getParameter (pe::params::ParametersProvider::getInstance().getCeiling().getId())), juce::Colour{ juce::Colours::green }, g);
    drawTicks (mTicks, juce::Colour{ juce::Colours::orange }.withAlpha (0.5f), g);
    // drawLevels ("Input", mInputLevelMeter, g);
}

void ClipMeter::drawBuffer (std::deque<float>& buffer, juce::Colour&& colour, juce::Graphics& g)
{
    auto const bounds = getBounds();
    auto const width = static_cast<float> (bounds.getWidth());
    auto const height = static_cast<float> (bounds.getHeight());

    juce::Path p;

    g.setColour (colour.withAlpha (1.0f));
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

void ClipMeter::drawDbLine (float const& dB, juce::Colour&& colour, juce::Graphics& g)
{
    auto const bounds = getBounds();
    auto const width = static_cast<float> (bounds.getWidth());
    auto const height = static_cast<float> (bounds.getHeight());
    auto const yPos = gDbToYPos (dB, height);
    juce::Point<float> start (0.0f, yPos);
    juce::Point<float> end (width, yPos);
    juce::Line<float> line (start, end);
    g.setColour (colour);
    g.drawLine (line, 4.0f);
}

void ClipMeter::drawLevels (std::string const& name, std::shared_ptr<pe::dsp::LevelMeter<float>> levelMeter, juce::Graphics& g)
{
    int const poxX = 20;
    int const width = 200;
    int const height = 40;
    std::string const ampLeft = "Amp(L): " + std::to_string (std::get<0> (levelMeter->getAmplification()));
    std::string const ampRight = "Amp(R): " + std::to_string (std::get<1> (levelMeter->getAmplification()));
    std::string const db = "dB: " + std::to_string (levelMeter->getDecibels());
    g.setColour (juce::Colour (juce::Colours::orange));
    g.drawText (name + ":", poxX, 20, width, height, juce::Justification::left, true);
    g.drawText (ampLeft, poxX, 40, width, height, juce::Justification::left, true);
    g.drawText (ampRight, poxX, 60, width, height, juce::Justification::left, true);
    g.drawText (db, poxX, 80, width, height, juce::Justification::left, true);
}

void ClipMeter::drawTicks (std::vector<float> const& ticksLevels, juce::Colour&& colour, juce::Graphics& g)
{
    auto const bounds = getBounds();
    auto const height = static_cast<float> (bounds.getHeight());
    auto const tickWidth = 25;
    g.setColour (colour);
    for (auto const& tickLevel : ticksLevels)
    {
        auto const yPos = gDbToYPos (tickLevel, height);
        juce::Point<float> start (0.0f, yPos);
        juce::Point<float> end (tickWidth, yPos);
        juce::Line<float> line (start, end);
        g.drawLine (line, 1.0f);
        std::string const db = std::to_string (static_cast<int> (tickLevel)) + "dB";
        g.setFont (9.0f);
        g.drawText (db, 0, static_cast<int> (yPos) - 15, tickWidth, 20, juce::Justification::left, true);
    }
}

void ClipMeter::resized()
{
}

void ClipMeter::onTimerTick()
{
    repaint();
}
} // namespace gui
} // namespace pe