#pragma once

#include <juce_graphics/juce_graphics.h>

namespace pe::gui::colourscheme
{
static juce::Colour const BackgroundPrimary = juce::Colour::fromRGB (30, 30, 30);
static juce::Colour const BackgroundSecondary = juce::Colour::fromRGB (50, 50, 50);
static juce::Colour const BackgroundTertiary = juce::Colour::fromRGB (179, 179, 179);
static juce::Colour const ForegroundPrimary = juce::Colour::fromRGB (47, 132, 175);
static juce::Colour const ForegroundSecondary = juce::Colour::fromRGB (171, 14, 54);
static juce::Colour const ForegroundTertiary = juce::Colour::fromRGB (28, 212, 184);
static juce::Colour const Warning = juce::Colour::fromRGB (255, 214, 0);
static juce::Colour const TextFocusLevel0 = juce::Colour::fromRGB (255, 255, 255);
static juce::Colour const TextFocusLevel1 = juce::Colour::fromRGBA (255, 255, 255, 178);
static juce::Colour const TextFocusLevel2 = juce::Colour::fromRGBA (255, 255, 255, 128);
static juce::Colour const TextFocusLevel3 = juce::Colour::fromRGBA (255, 255, 255, 97);
} // namespace pe::gui::colourscheme
