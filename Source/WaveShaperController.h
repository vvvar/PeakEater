/*
  ==============================================================================

    WaveShaperController.h
    Created: 3 Feb 2021 1:30:15pm
    Author:  Vladislav Voinov

  ==============================================================================
*/
#pragma once

#include "WaveShaping/OversampledWaveShaper.h"

namespace controller
{

//==============================================================================
/* Struct to hold all available WaveShaper parameters */
struct WaveShaperPrameters
{
    float inputGain;
    float outputGain;
    float ceiling;
    int   clippingType;
    int   oversampleRate;
};

//==============================================================================
template<typename T>
class WaveShaperController
{
public:
    //==============================================================================
    using WaveShaperPtr = std::shared_ptr<waveshaping::OversampledWaveShaper<T>>;
    
    //==============================================================================
    WaveShaperController (WaveShaperPtr myWaveShaperPtr) noexcept :
        waveShaperPtr (myWaveShaperPtr)
    {}
    
    //==============================================================================
    void handleParametersChange (WaveShaperPrameters parameters) noexcept
    {
        if (parameters.inputGain != previousInputGain)
        {
            previousInputGain = parameters.inputGain;
            waveShaperPtr->setInputGain (previousInputGain);
        }
        if (parameters.clippingType != previousClippingType)
        {
            previousClippingType = parameters.clippingType;
            waveShaperPtr->setClippingType (parameterChoiceToClippingType (previousClippingType));
        }
        if (parameters.ceiling != previousCeiling)
        {
            previousCeiling = parameters.ceiling;
            waveShaperPtr->setCeiling (previousCeiling);
        }
        if (parameters.oversampleRate != previousOversampleRate)
        {
            previousOversampleRate = parameters.oversampleRate;
            waveShaperPtr->setOversamplingRate (parameterChoiceToOversampleRate (previousOversampleRate));
        }
        if (parameters.outputGain != previousOutputGain)
        {
            previousOutputGain = parameters.outputGain;
            waveShaperPtr->setOutputGain (previousOutputGain);
        }
    }
    
    void prepare (WaveShaperPrameters parameters) noexcept
    {
        previousInputGain = parameters.inputGain;
        previousClippingType = parameters.clippingType;
        previousCeiling = parameters.ceiling;
        previousOutputGain = parameters.outputGain;
        previousOversampleRate = parameters.oversampleRate;

        waveShaperPtr->setInputGain (previousInputGain);
        waveShaperPtr->setClippingType (parameterChoiceToClippingType (previousClippingType));
        waveShaperPtr->setCeiling (previousCeiling);
        waveShaperPtr->setOversamplingRate (parameterChoiceToOversampleRate (previousOversampleRate));
        waveShaperPtr->setOutputGain (previousOutputGain);
    }
    
private:
    //==============================================================================
    WaveShaperPtr waveShaperPtr;
    
    //==============================================================================
    bool  previousLinkInOut      = false;
    float previousInputGain      = 0.0f;
    float previousOutputGain     = 0.0f;
    float previousCeiling        = 0.0f;
    int   previousClippingType   = 0;
    int   previousOversampleRate = 0;
    
    //==============================================================================
    waveshaping::ClippingType parameterChoiceToClippingType(const unsigned int parameterChoice) const noexcept
    {
        switch (parameterChoice)
        {
            case 0:
                return waveshaping::ClippingType::HARD;
            case 1:
                return waveshaping::ClippingType::QUINTIC;
            case 2:
                return waveshaping::ClippingType::CUBIC;
            case 3:
                return waveshaping::ClippingType::HYPERBOLIC_TAN;
            case 4:
                return waveshaping::ClippingType::ALGEBRAIC;
            case 5:
                return waveshaping::ClippingType::ARCTANGENT;
            default:
                return waveshaping::ClippingType::HARD;
        }
    }
        
    waveshaping::OversamplingRate parameterChoiceToOversampleRate(const unsigned int parameterChoice) const noexcept
    {
        switch (previousOversampleRate)
        {
            case 0:
                return waveshaping::OversamplingRate::X2;
            case 1:
                return waveshaping::OversamplingRate::X4;
            case 2:
                return waveshaping::OversamplingRate::X8;
            case 3:
                return waveshaping::OversamplingRate::X16;
            default:
                return waveshaping::OversamplingRate::X2;
        }
    }
};

}
