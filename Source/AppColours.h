#pragma once
#include <JuceHeader.h>

//==============================================================================
namespace AppColors
{

// Theme v1
//==============================================================================
static const juce::Colour Paper   { 250, 250, 250 };
static const juce::Colour Paper2  { 241, 241, 230 };
static const juce::Colour Red     { 210, 14, 44 };
static const juce::Colour Navy    { 0, 29, 45 };
static const juce::Colour Blue    { 156, 174, 188 };
static const juce::Colour Yellow  { 234, 153, 39 };
static const juce::Colour Green   { 100, 181, 100 };
static const juce::Colour Green2  { 0, 45, 16 };
static const juce::Colour Pastele { 255, 235, 204 };
static const juce::Colour Grey    { 47, 72, 88 };

namespace V2
{
// Theme v2
//==============================================================================
static const juce::Colour Black       { 0, 1, 3 };
static const juce::Colour LightGrey   { 203, 203, 205 };
static const juce::Colour DavysGrey   { 81, 80, 82 };
static const juce::Colour Jet         { 51, 49, 56 };
static const juce::Colour Powder      { 255, 255, 250 };
static const juce::Colour TiffanyBlue { 46, 196, 182 };
static const juce::Colour RedRYB      { 255, 49, 46 };
static const juce::Colour Transparent = Black.withAlpha (0.0f);

}


};
