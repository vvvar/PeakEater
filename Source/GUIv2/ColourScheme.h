#pragma once
#include <juce_graphics/juce_graphics.h>

namespace pe
{
namespace gui
{
namespace colourscheme
{
static auto BackgroundPrimary = juce::Colour (30, 30, 30);
static auto BackgroundSecondary = juce::Colour (50, 50, 50);
static auto BackgroundTertiary = juce::Colour (179, 179, 179);

static auto ForegroundPrimary = juce::Colour (0, 76, 152);
static auto ForegroundSecondary = juce::Colour (196, 0, 0).withAlpha(0.6f);
static auto ForegroundTertiary = juce::Colour (28, 212, 184);

static auto Warning = juce::Colour (255, 214, 0);

static auto TextFocusLevel0 = juce::Colour (255, 255, 255);
static auto TextFocusLevel1 = juce::Colour (255, 255, 255).withAlpha(0.70f);
static auto TextFocusLevel2 = juce::Colour (255, 255, 255).withAlpha(0.50f);
static auto TextFocusLevel3 = juce::Colour (255, 255, 255).withAlpha(0.38f);
} // colourscheme
} // gui
} // pe