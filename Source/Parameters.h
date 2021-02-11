#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 * Helper namespace to keep consistent parameter info
 * across all project. For example, to keep label text similar
 * in the automation section and in the UI.
 */
namespace Parameters
{

using String            = juce::String;
using NormalisableRange = juce::NormalisableRange<float>;
using StringArray       = juce::StringArray;

struct RangeInfo
{
    NormalisableRange Range        { -1.0f, 1.0f, 0.1f };
    float             DefaultValue { 0.0f };
};

struct ChoiceInfo
{
    StringArray Choices { " " };
    int         DefaultChoiceId { 0 };
};

struct BoolInfo
{
    bool DefaultValue { false };
};

struct ParameterInfo
{
    String     Id;
    String     Label;
    RangeInfo  RangeInfo;
    ChoiceInfo ChoicesInfo;
    BoolInfo   BoolInfo;
};
 
static const RangeInfo  InputGainRange   = { { -36.0f, 36.0f, 0.1f, 0.6f, true } };
static const RangeInfo  OutputGainRange  = { { -36.0f, 36.0f, 0.1f, 0.6f, true } };
static const RangeInfo  CeilingRange     = { { -36.0f, 0.0f, 0.1f, 0.6f }, 0.0f };
static const ChoiceInfo ClippingTypeInfo = { {
    "HARD",
    "QUINTIC",
    "CUBIC",
    "TANGENT",
    "ALGEBRAIC",
    "ARCTANGENT"
}, 0 };
static const ChoiceInfo OversampleRateInfo = { { "x2", "x4", "x8", "x16" }, 0 };

static const ParameterInfo InputGain      { "InputGain", "INPUT GAIN", InputGainRange };
static const ParameterInfo OutputGain     { "OutputGain", "OUTPUT GAIN", OutputGainRange };
static const ParameterInfo LinkInOut      { "LinkInOut", "IN<->OUT" };
static const ParameterInfo Bypass         { "Bypass", "BYPASS" };
static const ParameterInfo Ceiling        { "Ceiling", "CEILING", CeilingRange };
static const ParameterInfo ClippingType   { "ClippingType", "ALGORYTHM", {}, ClippingTypeInfo };
static const ParameterInfo OversampleRate { "OversampleRate", "OVERSAMPLE", {}, OversampleRateInfo };

}
