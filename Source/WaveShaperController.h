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
struct WaveShaperPrameters
{
    bool  linkInOut;
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
    WaveShaperController(WaveShaperPtr myWaveShaperPtr) noexcept :
        waveShaperPtr(myWaveShaperPtr)
    {}
    
    //==============================================================================
    void handleParametersChange(WaveShaperPrameters parameters)
    {
        if (parameters.linkInOut != previousLinkInOut)
        {
            previousLinkInOut = parameters.linkInOut;
            
            previousInputGain = parameters.inputGain;
            previousOutputGain = previousInputGain;
            
            waveShaperPtr->setInputGain(previousInputGain);
            waveShaperPtr->setOutputGain(previousInputGain);
        }
        if (previousLinkInOut)
        {
            if (parameters.inputGain != previousInputGain)
            {
                previousInputGain = parameters.inputGain;
                waveShaperPtr->setInputGain(previousInputGain);
                waveShaperPtr->setOutputGain(previousInputGain);
            }
        } else
        {
            if (parameters.inputGain != previousInputGain)
            {
                previousInputGain = parameters.inputGain;
                waveShaperPtr->setInputGain(previousInputGain);
            }
            if (parameters.outputGain != previousOutputGain)
            {
                previousOutputGain = parameters.outputGain;
                waveShaperPtr->setOutputGain(previousOutputGain);
            }
        }
        if (parameters.ceiling != previousCeiling)
        {
            previousCeiling = parameters.ceiling;
            waveShaperPtr->setCeiling(previousCeiling);
        }
        if (parameters.clippingType != previousClippingType)
        {
            previousClippingType = parameters.clippingType;
            switch (previousClippingType)
            {
                case 0:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::LOGARYTHMIC);
                    break;
                case 1:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::HARD);
                    break;
                case 2:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::QUINTIC);
                    break;
                case 3:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::CUBIC);
                    break;
                case 4:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::HYPERBOLIC_TAN);
                    break;
                case 5:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::ALGEBRAIC);
                    break;
                case 6:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::ARCTANGENT);
                    break;
                case 7:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::SIN);
                    break;
                case 8:
                    waveShaperPtr->setClippingType(waveshaping::ClippingType::LIMIT);
                    break;
                default:
                    break;
            }
        }
        if (parameters.oversampleRate != previousOversampleRate)
        {
            previousOversampleRate = parameters.oversampleRate;
            switch (previousOversampleRate)
            {
                case 0:
                    waveShaperPtr->setOversamplingRate(waveshaping::OversamplingRate::X2);
                    break;
                case 1:
                    waveShaperPtr->setOversamplingRate(waveshaping::OversamplingRate::X4);
                    break;
                case 2:
                    waveShaperPtr->setOversamplingRate(waveshaping::OversamplingRate::X8);
                    break;
                case 3:
                    waveShaperPtr->setOversamplingRate(waveshaping::OversamplingRate::X16);
                    break;
                default:
                    waveShaperPtr->setOversamplingRate(waveshaping::OversamplingRate::X2);
                    break;
            }
        }
    }
    
private:
    //==============================================================================
    WaveShaperPtr waveShaperPtr;
    
    //==============================================================================
    bool  previousLinkInOut      = false;
    float previousInputGain      = 0.0f;
    float previousOutputGain     = 0.0f;
    float previousCeiling        = -0.1f;
    int   previousClippingType   = 0;
    int   previousOversampleRate = 0;
};

}
