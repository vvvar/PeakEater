#pragma once

#include <juce_core/juce_core.h>

//==============================================================================
/*
 * Helper namespace to keep consistent parameter info
 * across all project. For example, to keep label text similar
 * in the automation section and in the UI.
 */
namespace pe
{
namespace params
{

class Parameter
{
public:
    Parameter (juce::String const& id, juce::String const& label)
        : mId (id)
        , mLabel (label)
    {
    }

    juce::String getId() const
    {
        return mId;
    }

    juce::String getLabel() const
    {
        return mLabel;
    }

private:
    juce::String mId;
    juce::String mLabel;
};

template <typename T>
class RangedParameter : public Parameter
{
public:
    RangedParameter (juce::String const& id, juce::String const& label, juce::NormalisableRange<T> const& range)
        : Parameter (id, label)
        , mRange (range)
    {
    }

    juce::NormalisableRange<T>& getRange()
    {
        return mRange;
    }

private:
    juce::NormalisableRange<T> mRange;
};

class ChoicingParameter : public Parameter
{
public:
    ChoicingParameter (juce::String const& id, juce::String const& label, juce::StringArray const&& choices)
        : Parameter (id, label)
        , mChoices (choices)
    {
    }

    juce::StringArray& getChoices()
    {
        return mChoices;
    }

private:
    juce::StringArray mChoices;
};

class ParametersProvider
{
public:
    static ParametersProvider& getInstance()
    {
        static ParametersProvider pp;
        return pp;
    }

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        return {
            std::make_unique<juce::AudioParameterFloat> (mInputGain.getId(), mInputGain.getLabel(), mInputGain.getRange(), 0.0f),
            std::make_unique<juce::AudioParameterFloat> (mOutputGain.getId(), mOutputGain.getLabel(), mOutputGain.getRange(), 0.0f),
            std::make_unique<juce::AudioParameterFloat> (mCeiling.getId(), mCeiling.getLabel(), mCeiling.getRange(), 0.0f),
            std::make_unique<juce::AudioParameterBool> (mLinkInOut.getId(), mLinkInOut.getLabel(), false),
            std::make_unique<juce::AudioParameterBool> (mBypass.getId(), mBypass.getLabel(), false),
            std::make_unique<juce::AudioParameterChoice> (mClippingType.getId(), mClippingType.getLabel(), mClippingType.getChoices(), 0),
            std::make_unique<juce::AudioParameterChoice> (mOversampleRate.getId(), mOversampleRate.getLabel(), mOversampleRate.getChoices(), 0)
        };
    }

    RangedParameter<float>& getInputGain()
    {
        return mInputGain;
    }

    RangedParameter<float>& getOutputGain()
    {
        return mOutputGain;
    }

    RangedParameter<float>& getCeiling()
    {
        return mCeiling;
    }

    Parameter& getBypass()
    {
        return mBypass;
    }

    Parameter& getLinkInOut()
    {
        return mLinkInOut;
    }

    ChoicingParameter& getClippingType()
    {
        return mClippingType;
    }

    ChoicingParameter& getOversampleRate()
    {
        return mOversampleRate;
    }

private:
    ParametersProvider()
        : mInputGain ("InputGain", "INPUT", { -36.0f, 36.0f, 0.1f, 0.5f, true })
        , mOutputGain ("OutputGain", "OUTPUT", { -36.0f, 36.0f, 0.1f, 0.5f, true })
        , mCeiling ("Ceiling", "CEILING", { -36.0f, 0.0f, 0.1f, 1.9f, false })
        , mBypass ("Bypass", "BYPASS")
        , mLinkInOut ("LinkInOut", "IN<->OUT")
        , mClippingType ("ClippingType", "ALGORITHM", { "HARD", "QUINTIC", "CUBIC", "TANGENT", "ALGEBRAIC", "ARCTANGENT" })
        , mOversampleRate ("OversampleRate", "OVERSAMPLE", { "x2", "x4", "x8", "x16" })
    {
    }
    ~ParametersProvider()
    {
    }

    RangedParameter<float> mInputGain;
    RangedParameter<float> mOutputGain;
    RangedParameter<float> mCeiling;
    Parameter mBypass;
    Parameter mLinkInOut;
    ChoicingParameter mClippingType;
    ChoicingParameter mOversampleRate;
};
} // namespace params
} // namespace pe
