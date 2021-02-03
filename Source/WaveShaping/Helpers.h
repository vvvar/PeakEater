/*
  ==============================================================================

    Helpers.h
    Created: 3 Feb 2021 12:29:17pm
    Author:  Vladyslav Voinov

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

namespace waveshaping
{

template<typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

}
