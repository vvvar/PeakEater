#include "GainDialLookAndFeel.h"

namespace pe
{
namespace gui
{
void GainDialLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
	DialLookAndFeel::drawRotarySlider (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
	drawProgress (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
	drawStick (g, x, y, width, height, sliderPos, rotaryStartAngle, rotaryEndAngle, slider);
}

void GainDialLookAndFeel::drawProgress (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
	auto bounds = juce::Rectangle<float> (x, y, width, height).reduced (2.0f);
	auto radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
	auto lineW = radius * 0.085f;
	auto arcRadius = radius - lineW * 1.6f;
	auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	auto canterAngle = rotaryStartAngle + 0.5f * (rotaryEndAngle - rotaryStartAngle); // 0.5f - central slider position

	// Draw bi-directional progress
	juce::Path valueArc;
	valueArc.addCentredArc (bounds.getCentreX(),
	                        bounds.getCentreY(),
	                        arcRadius,
	                        arcRadius,
	                        0.0f,
	                        canterAngle,
	                        toAngle,
	                        true);
	g.setColour (juce::Colours::aquamarine);
	g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	// Draw uniderectional progress
	// juce::Path valueArc;
	// valueArc.addCentredArc (bounds.getCentreX(),
	//                         bounds.getCentreY(),
	//                         arcRadius,
	//                         arcRadius,
	//                         0.0f,
	//                         rotaryStartAngle,
	//                         toAngle,
	//                         true);
	// g.setColour (juce::Colours::aquamarine);
	// g.strokePath (valueArc, juce::PathStrokeType (lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}
} // namespace gui
} // namespace pe