#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
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
        Parameter (int const hint, juce::String const& id, juce::String const& label)
            : mParameterID (id, hint), mLabel (label)
        {
        }

        [[nodiscard]] juce::ParameterID const& getId() const { return mParameterID; }

        juce::String getLabel() const { return mLabel; }

    private:
        juce::ParameterID const mParameterID;
        juce::String mLabel;
    };

    template <typename T>
    class RangedParameter : public Parameter
    {
    public:
        RangedParameter (int const hint,
                         juce::String const& id,
                         juce::String const& label,
                         juce::NormalisableRange<T> const&& range)
            : Parameter (hint, id, label), mRange (range)
        {
        }

        juce::NormalisableRange<T>& getRange() { return mRange; }

    private:
        juce::NormalisableRange<T> mRange;
    };

    class ChoicingParameter : public Parameter
    {
    public:
        ChoicingParameter (int const hint,
                           juce::String const& id,
                           juce::String const& label,
                           juce::StringArray const&& choices)
            : Parameter (hint, id, label), mChoices (choices)
        {
        }

        juce::StringArray& getChoices() { return mChoices; }

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
                std::make_unique<juce::AudioParameterFloat> (
                    mInputGain.getId(), mInputGain.getLabel(), mInputGain.getRange(), 0.0f),
                std::make_unique<juce::AudioParameterFloat> (
                    mOutputGain.getId(), mOutputGain.getLabel(), mOutputGain.getRange(), 0.0f),
                std::make_unique<juce::AudioParameterFloat> (
                    mCeiling.getId(), mCeiling.getLabel(), mCeiling.getRange(), 0.0f),
                std::make_unique<juce::AudioParameterBool> (mLinkInOut.getId(), mLinkInOut.getLabel(), false),
                std::make_unique<juce::AudioParameterBool> (mBypass.getId(), mBypass.getLabel(), false),
                std::make_unique<juce::AudioParameterChoice> (
                    mClippingType.getId(), mClippingType.getLabel(), mClippingType.getChoices(), 0),
                std::make_unique<juce::AudioParameterChoice> (
                    mOversampleRate.getId(), mOversampleRate.getLabel(), mOversampleRate.getChoices(), 0),
                std::make_unique<juce::AudioParameterFloat> (
                    mDryWet.getId(), mDryWet.getLabel(), mDryWet.getRange(), 1.0f),
            };
        }

        RangedParameter<float>& getInputGain() { return mInputGain; }

        RangedParameter<float>& getOutputGain() { return mOutputGain; }

        RangedParameter<float>& getCeiling() { return mCeiling; }

        Parameter& getBypass() { return mBypass; }

        Parameter& getLinkInOut() { return mLinkInOut; }

        ChoicingParameter& getClippingType() { return mClippingType; }

        ChoicingParameter& getOversampleRate() { return mOversampleRate; }

        RangedParameter<float>& getDryWet() { return mDryWet; }

    private:
        ParametersProvider()
            : mInputGain (1, "InputGain", "INPUT", { -36.0f, 36.0f, 0.1f, 0.5f, true }),
              mOutputGain (2, "OutputGain", "OUTPUT", { -36.0f, 36.0f, 0.1f, 0.5f, true }),
              mCeiling (3, "Ceiling", "CEILING", { -36.0f, 0.0f, 0.1f, 1.9f, false }), mBypass (4, "Bypass", "BYPASS"),
              mLinkInOut (5, "LinkInOut", "INOUT"),
              mClippingType (6,
                             "ClippingType",
                             "ALGORITHM",
                             { "HARD", "QUINTIC", "CUBIC", "TANGENT", "ALGEBRAIC", "ARCTANGENT" }),
              mOversampleRate (7, "OversampleRate", "OVERSAMPLE", { "OFF", "x2", "x4", "x8", "x16", "x32" }),
              mDryWet (8, "DryWet", "DRYWET", { 0.0f, 1.0f, 0.001f })
        {
        }
        ~ParametersProvider() {}

        RangedParameter<float> mInputGain;
        RangedParameter<float> mOutputGain;
        RangedParameter<float> mCeiling;
        Parameter mBypass;
        Parameter mLinkInOut;
        ChoicingParameter mClippingType;
        ChoicingParameter mOversampleRate;
        RangedParameter<float> mDryWet;
    };
} // namespace params
} // namespace pe
