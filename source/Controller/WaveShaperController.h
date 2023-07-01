/*
   ==============================================================================

    WaveShaperController.h
    Created: 3 Feb 2021 1:30:15pm
    Author:  Vladislav Voinov

   ==============================================================================
 */
#pragma once

#include "../DSP/OversampledWaveShaper.h"

namespace pe
{
namespace controller
{
    //==============================================================================
    /* Struct to hold all available WaveShaper parameters */
    struct WaveShaperPrameters
    {
        float inputGain;
        float outputGain;
        float ceiling;
        int clippingType;
        int oversampleRate;
    };

    //==============================================================================
    template <typename T>
    class WaveShaperController
    {
    public:
        //==============================================================================
        WaveShaperController()
            : waveShaper(), previousLinkInOut (false), previousInputGain (0.0f), previousOutputGain (0.0f),
              previousCeiling (0.0f), previousClippingType (0), previousOversampleRate (0)
        {
        }

        //==============================================================================
        void onPostInputGain (dsp::waveshaping::OversampledWaveShaperEventHandler const& handler)
        {
            waveShaper.onPostInputGain (handler);
        }

        void onPostCeiling (dsp::waveshaping::OversampledWaveShaperEventHandler const& handler)
        {
            waveShaper.onPostCeiling (handler);
        }

        void onPostOutputGain (dsp::waveshaping::OversampledWaveShaperEventHandler const& handler)
        {
            waveShaper.onPostOutputGain (handler);
        }

        void unsubscribeFromAll() { waveShaper.unsubscribeFromAll(); }

        void handleParametersChange (WaveShaperPrameters parameters) noexcept
        {
            if (parameters.inputGain != previousInputGain)
            {
                previousInputGain = parameters.inputGain;
                waveShaper.setInputGain (previousInputGain);
            }
            if (parameters.clippingType != previousClippingType)
            {
                previousClippingType = parameters.clippingType;
                waveShaper.setClippingType (parameterChoiceToClippingType (previousClippingType));
            }
            if (parameters.ceiling != previousCeiling)
            {
                previousCeiling = parameters.ceiling;
                waveShaper.setCeiling (previousCeiling);
            }
            if (parameters.oversampleRate != previousOversampleRate)
            {
                previousOversampleRate = parameters.oversampleRate;
                waveShaper.setOversamplingRate (parameterChoiceToOversampleRate (previousOversampleRate));
            }
            if (parameters.outputGain != previousOutputGain)
            {
                previousOutputGain = parameters.outputGain;
                waveShaper.setOutputGain (previousOutputGain);
            }
        }

        void prepare (WaveShaperPrameters const&& parameters, juce::dsp::ProcessSpec const& spec) noexcept
        {
            waveShaper.prepare (spec);

            previousInputGain = parameters.inputGain;
            previousClippingType = parameters.clippingType;
            previousCeiling = parameters.ceiling;
            previousOutputGain = parameters.outputGain;
            previousOversampleRate = parameters.oversampleRate;

            waveShaper.setInputGain (previousInputGain);
            waveShaper.setClippingType (parameterChoiceToClippingType (previousClippingType));
            waveShaper.setCeiling (previousCeiling);
            waveShaper.setOversamplingRate (parameterChoiceToOversampleRate (previousOversampleRate));
            waveShaper.setOutputGain (previousOutputGain);
        }

        void process (juce::AudioBuffer<T>& buffer) { waveShaper.process (buffer); }

        void reset() { waveShaper.reset(); }

        float getPreviousInputGain() const { return previousInputGain; }

        float getPreviousOutputGain() const { return previousOutputGain; }

    private:
        //==============================================================================
        dsp::waveshaping::OversampledWaveShaper<T> waveShaper;

        //==============================================================================
        bool previousLinkInOut;
        float previousInputGain;
        float previousOutputGain;
        float previousCeiling;
        int previousClippingType;
        int previousOversampleRate;

        //==============================================================================
        dsp::waveshaping::ClippingType parameterChoiceToClippingType (int const& parameterChoice) const
        {
            using ClippingType = dsp::waveshaping::ClippingType;
            switch (parameterChoice)
            {
                case 0:
                    return ClippingType::HARD;
                case 1:
                    return ClippingType::QUINTIC;
                case 2:
                    return ClippingType::CUBIC;
                case 3:
                    return ClippingType::HYPERBOLIC_TAN;
                case 4:
                    return ClippingType::ALGEBRAIC;
                case 5:
                    return ClippingType::ARCTANGENT;
                default:
                    return ClippingType::HARD;
            }
        }

        dsp::waveshaping::OversamplingRate parameterChoiceToOversampleRate (int const& parameterChoice) const
        {
            using OversamplingRate = dsp::waveshaping::OversamplingRate;
            switch (parameterChoice)
            {
                case 0:
                    return OversamplingRate::X1;
                case 1:
                    return OversamplingRate::X2;
                case 2:
                    return OversamplingRate::X4;
                case 3:
                    return OversamplingRate::X8;
                case 4:
                    return OversamplingRate::X16;
                case 5:
                    return OversamplingRate::X32;
                default:
                    return OversamplingRate::X1;
            }
        }
    };
} // namespace controller
} // namespace pe
