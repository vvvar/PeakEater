#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <string>

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
inline float gRoundDb (float const& dB)
{
    return ((float) ((int) (dB * 10))) / 10;
}
template <typename T>
inline std::string gToStringWithPrecision (const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision (n);
    out << std::fixed << a_value;
    return out.str();
}
} // namespace gui
} // namespace pe