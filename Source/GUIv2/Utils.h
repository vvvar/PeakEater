#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
namespace
{
auto constexpr fontScalingCoefMed = 110.0f;
auto constexpr fontScalingCoefLarge = 90.0f;
} // namespace
inline int calculateTextSize (int const& windowWidth, int const& windowHeight)
{
    return (windowWidth + windowHeight) / fontScalingCoefMed;
}

inline int calculatePrimaryTextSize (int const& windowWidth, int const& windowHeight)
{
    return (windowWidth + windowHeight) / fontScalingCoefLarge;
}
} // namespace gui
} // namespace pe