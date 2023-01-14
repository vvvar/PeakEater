#include "AnalyserComponent.h"

#include <JuceHeader.h>

#include "../Utils.h"
#include "../ColourScheme.h"

namespace pe
{
namespace gui
{
namespace
{
// DSP configuration
int constexpr gAnalyzerSampleRateHz = 30;
float constexpr gMinDbValInOut = -36.0f;
float constexpr gMinDbValEaten = 0.0f;
// GUI configuration
float constexpr gBorderWidth = 1.0f;
float constexpr gBorderRadius = 3.0f;

template <typename T>
bool gIsInBounds (const T& value, const T& low, const T& high)
{
	return !(value < low) && (value < high);
}
} // namespace
AnalyserComponent::AnalyserComponent (std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
                                      std::shared_ptr<pe::dsp::LevelMeter<float> > inputLevelMeter,
                                      std::shared_ptr<pe::dsp::LevelMeter<float> > clippingLevelMeter,
                                      std::shared_ptr<pe::dsp::LevelMeter<float> > outputLevelMeter)
	: mInputLevelMeter (inputLevelMeter)
	, mClippingLevelMeter (clippingLevelMeter)
	, mOutputLevelMeter (outputLevelMeter)
	, mInputAnalyzer (gAnalyzerSampleRateHz, gMinDbValInOut)
	, mOutputAnalyzer (gAnalyzerSampleRateHz, gMinDbValInOut)
	, mEatenAnalyzer (gAnalyzerSampleRateHz, gMinDbValEaten)
	, mAnalyzerUpdateTimer (std::bind (&AnalyserComponent::onAnalyzerUpdateTick, this))
{
	setMouseCursor (juce::MouseCursor::PointingHandCursor);
	mAnalyzerUpdateTimer.startTimerHz (gAnalyzerSampleRateHz);
}

AnalyserComponent::~AnalyserComponent()
{
	setLookAndFeel (nullptr);
}

void AnalyserComponent::resized()
{
}

void AnalyserComponent::paint (juce::Graphics& g)
{
	drawLevels (mInputAnalyzer.getMagnitude(), mOutputAnalyzer.getMagnitude(), mEatenAnalyzer.getMagnitude(), g);
}

void AnalyserComponent::drawLevels (float inputLevel,
                                    float outputLevel,
                                    float eatenAmount,
                                    juce::Graphics& g)
{
	auto const bounds = getBounds().reduced (gBorderWidth).toFloat();
	auto const width = bounds.getWidth();
	auto const height = bounds.getHeight();

	int const poxX = 0;
	int const posY = 0;
	int const boxWidth = width;
	int const boxHeight = height;
	auto const fontSize = calculatePrimaryTextSize (getTopLevelComponent()->getBounds().getWidth(), getTopLevelComponent()->getBounds().getHeight());
	auto const newlinePadding = fontSize * 0.5f;

	juce::Colour borderColor = colourscheme::BackgroundSecondary;
	juce::Colour backgroundColor = colourscheme::BackgroundSecondary;
	juce::Colour textColor = colourscheme::TextFocusLevel0;
	if (!isEnabled())
	{
		// borderColor = borderColor.withAlpha (0.5f);
		// backgroundColor = backgroundColor.withAlpha (0.5f);
		textColor = colourscheme::TextFocusLevel3;
	}

	const juce::Rectangle<float> area (poxX, posY, boxWidth, boxHeight);
	g.setColour (backgroundColor);
	g.fillRoundedRectangle (area, gBorderRadius);
	// g.setColour (borderColor);
	// g.drawRoundedRectangle (area, gBorderRadius, gBorderWidth);

	std::string const inputLevelText = "Input: " + gToStringWithPrecision (gRoundDb (inputLevel), 1) + " dB";
	std::string const outLevelText = "Output: " + gToStringWithPrecision (gRoundDb (outputLevel), 1) + " dB";
	std::string const eatenLevelText = "Eaten: " + gToStringWithPrecision (gRoundDb (eatenAmount), 1) + " dB";
	g.setFont (fontSize);
	g.setColour (textColor);
	g.drawText (inputLevelText, poxX + newlinePadding, posY + newlinePadding, boxWidth, fontSize, juce::Justification::left, true);
	g.drawText (outLevelText, poxX + newlinePadding, posY + newlinePadding + fontSize + newlinePadding, boxWidth, fontSize, juce::Justification::left, true);
	if (gIsInBounds (eatenAmount, 5.0f, 10.0f))
	{
		g.setColour (colourscheme::Warning);
	}
	else if (gIsInBounds (eatenAmount, 10.0f, std::numeric_limits<float>::max()))
	{
		g.setColour (colourscheme::ForegroundSecondary);
	}
	else
	{
		g.setColour (textColor);
	}
	g.drawText (eatenLevelText, poxX + newlinePadding, posY + newlinePadding + (fontSize * 2) + (newlinePadding * 2), boxWidth, fontSize, juce::Justification::left, true);
}

void AnalyserComponent::mouseDown (juce::MouseEvent const& event)
{
	if (event.getNumberOfClicks() == 2)
	{
		mInputAnalyzer.reset();
		mOutputAnalyzer.reset();
		mEatenAnalyzer.reset();
		repaint();
	} else if (event.getNumberOfClicks() == 4) // Only for debug, to show current version
	{
		std::string const lModalBoxTitle = std::string(ProjectInfo::projectName) + " by " + std::string(ProjectInfo::companyName);
		std::string const lModalBoxText = "Version: " + std::string(ProjectInfo::versionString);
		juce::NativeMessageBox::showMessageBoxAsync(juce::AlertWindow::InfoIcon, lModalBoxTitle, lModalBoxText, nullptr, nullptr);
	}
}

void AnalyserComponent::onAnalyzerUpdateTick()
{
	auto const inputLevel = static_cast<float> (mInputLevelMeter->getDecibels());
	auto const outputLevel = static_cast<float> (mOutputLevelMeter->getDecibels());
	auto const clippingLevel = static_cast<float> (mClippingLevelMeter->getDecibels());
	mInputAnalyzer.push (inputLevel);
	mOutputAnalyzer.push (outputLevel);
	mEatenAnalyzer.push (inputLevel - clippingLevel);
}
} // namespace gui
} // namespace pe