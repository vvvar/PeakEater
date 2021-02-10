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
    WaveShaperController (WaveShaperPtr myWaveShaperPtr) noexcept :
        waveShaperPtr (myWaveShaperPtr)
    {}
    
    //==============================================================================
    void handleParametersChange (WaveShaperPrameters parameters) noexcept
    {
        if (parameters.linkInOut != previousLinkInOut)
        {
            previousLinkInOut = parameters.linkInOut;
            
            previousInputGain = parameters.inputGain;
            previousOutputGain = previousInputGain;
            
            waveShaperPtr->setInputGain (previousInputGain);
            setupOutputGain();
        }
        if (previousLinkInOut)
        {
            if (parameters.inputGain != previousInputGain)
            {
                previousInputGain = parameters.inputGain;
                waveShaperPtr->setInputGain (previousInputGain);
                setupOutputGain();
            }
        } else
        {
            if (parameters.inputGain != previousInputGain)
            {
                previousInputGain = parameters.inputGain;
                waveShaperPtr->setInputGain (previousInputGain);
            }
            if (parameters.outputGain != previousOutputGain)
            {
                previousOutputGain = parameters.outputGain;
                setupOutputGain();
            }
        }
        if (parameters.ceiling != previousCeiling)
        {
            previousCeiling = parameters.ceiling;
            waveShaperPtr->setCeiling (previousCeiling);
        }
        if (parameters.clippingType != previousClippingType)
        {
            previousClippingType = parameters.clippingType;
            setupClippingType();
        }
        if (parameters.oversampleRate != previousOversampleRate)
        {
            previousOversampleRate = parameters.oversampleRate;
            setupOversampleRate();
        }
    }
    
    void setup (WaveShaperPrameters parameters) noexcept
    {
        previousLinkInOut = parameters.linkInOut;
        previousInputGain = parameters.inputGain;
        previousOutputGain = parameters.outputGain;
        previousCeiling = parameters.ceiling;
        previousClippingType = parameters.clippingType;
        previousOversampleRate = parameters.oversampleRate;
        
        waveShaperPtr->setInputGain (previousInputGain);
        setupOutputGain();
        waveShaperPtr->setCeiling (previousCeiling);
        setupClippingType();
        setupOversampleRate();
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
    void setupClippingType()
    {
        switch (previousClippingType)
        {
            case 0:
                waveShaperPtr->setClippingType(waveshaping::ClippingType::HARD);
                break;
            case 1:
                waveShaperPtr->setClippingType(waveshaping::ClippingType::QUINTIC);
                break;
            case 2:
                waveShaperPtr->setClippingType(waveshaping::ClippingType::CUBIC);
                break;
            case 3:
                waveShaperPtr->setClippingType(waveshaping::ClippingType::HYPERBOLIC_TAN);
                break;
            case 4:
                waveShaperPtr->setClippingType(waveshaping::ClippingType::ALGEBRAIC);
                break;
            case 5:
                waveShaperPtr->setClippingType(waveshaping::ClippingType::ARCTANGENT);
                break;
            default:
                break;
        }
    }
        
    void setupOversampleRate()
    {
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
    
    void setupOutputGain()
    {
        if (previousLinkInOut)
        {
            if (previousInputGain > 0)
            {
                waveShaperPtr->setOutputGain (-previousInputGain);
            } else if (previousInputGain < 0)
            {
                waveShaperPtr->setOutputGain (std::fabs (previousInputGain));
            } else // input is 0
            {
                waveShaperPtr->setOutputGain (previousInputGain);
            }
        } else
        {
            waveShaperPtr->setOutputGain (previousOutputGain);
        }
    }
};

}
