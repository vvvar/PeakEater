#include "LinkInOut.h"
#include "../Utils.h"
#include <BinaryData.h>

namespace pe
{
namespace gui
{
namespace
{
auto constexpr gParamName = "LinkInOut";
}
LinkInOut::LinkInOut (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters)
    : mParameters (parameters)
    , mIsOn (false)
{
    mParameters->getParameter (gParamName)->addListener (this);
    setMouseCursor (juce::MouseCursor::PointingHandCursor);
}

LinkInOut::~LinkInOut()
{
    mParameters->getParameter (gParamName)->removeListener (this);
    setLookAndFeel (nullptr);
}

void LinkInOut::resized()
{
    // mToggle.setBounds (getLocalBounds());
}

void LinkInOut::paint (juce::Graphics& g)
{
    auto const paddingTopFactor = 0.2f;
    auto const paddingLeftFactor = 0.1f;
    auto const paddingRightFactor = 0.9f;

    auto const bounds = getLocalBounds();
    auto const width = static_cast<float> (bounds.getWidth());
    auto const height = static_cast<float> (bounds.getHeight());
    auto const centreX = static_cast<float> (bounds.getCentreX());
    auto const centreY = static_cast<float> (bounds.getCentreY()) + (height * paddingTopFactor);

    // Set color for lines and icon
    if (mIsOn)
    {
        g.setColour (juce::Colours::cyan);
    }
    else
    {
        g.setColour (juce::Colours::grey);
    }

    // Draw icon
    auto const icon = juce::ImageCache::getFromMemory (BinaryData::link_png, BinaryData::link_pngSize);
    auto const imgWidth = 14;
    auto const imgHeight = 14;
    auto const destX = static_cast<int> (centreX) - imgWidth / 2;
    auto const destY = static_cast<int> (centreY) - imgHeight / 2;
    g.drawImage (icon, destX, destY, imgWidth, imgHeight, 0, 0, icon.getWidth(), icon.getHeight(), true);

    // Draw curved line
    auto const topY = height * paddingTopFactor;
    // Left line part
    juce::Point<float> p1 (width * paddingLeftFactor, topY);
    juce::Point<float> p2 (width * paddingLeftFactor, centreY);
    juce::Point<float> p3 (width * 0.2f, centreY);
    juce::Point<float> p4 (centreX - imgWidth, centreY);
    juce::Path path1;
    path1.startNewSubPath (p1);
    path1.quadraticTo (p2, p3);
    path1.lineTo (p4);
    g.strokePath (path1, juce::PathStrokeType (1.0));
    // Right line part
    juce::Point<float> p5 (centreX + imgWidth, centreY);
    juce::Point<float> p6 (width * 0.8f, centreY);
    juce::Point<float> p7 (width * paddingRightFactor, centreY);
    juce::Point<float> p8 (width * paddingRightFactor, topY);
    juce::Path path2;
    path2.startNewSubPath (p5);
    path2.lineTo (p6);
    path2.quadraticTo (p7, p8);
    g.strokePath (path2, juce::PathStrokeType (1.0));

    // Draw label
    g.setColour (juce::Colour (juce::Colours::white));
    auto const fontSize = calculatePrimaryTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
    g.setFont (fontSize);
    g.drawText (juce::String ("Link Input with Output").toUpperCase(), 0, topY, width, height, juce::Justification::centredTop, true);
}

void LinkInOut::mouseDown (juce::MouseEvent const& event)
{
    std::cout << "Clicked at: " << event.getPosition().toString() << std::endl;
    // mIsOn = ! mIsOn;
    mParameters->getParameter (gParamName)->setValueNotifyingHost (! mIsOn);
}

void LinkInOut::parameterValueChanged (int parameterIndex, float newValue)
{
    mIsOn = mParameters->getParameter (gParamName)->getValue();
    repaint();
}

void LinkInOut::parameterGestureChanged (int parameterIndex, bool gestureIsStarting)
{
    mIsOn = mParameters->getParameter (gParamName)->getValue();
    repaint();
}
} // namespace gui
} // namespace pe