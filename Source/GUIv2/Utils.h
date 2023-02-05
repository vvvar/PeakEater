#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <string>
#include <map>

namespace pe
{
namespace gui
{
namespace
{
auto constexpr fontScalingCoefMed = 110;
auto constexpr fontScalingCoefLarge = 90;
static std::map<std::string, float> yPosCache = {};
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
inline float gDbToYPos (float const& dB, float const& maxY, bool isLinear, float const& maxDb = 36.0f)
{
	if (std::isinf (dB))
	{
		return maxY;
	}
	if (dB >= 0.0f)
	{
		return 0.0f;
	}
	std::string paramsHash = std::to_string(dB) + std::to_string(isLinear) + std::to_string(maxY);
	if (yPosCache.find(paramsHash) == yPosCache.end()) {
		if (isLinear)
		{
			yPosCache.emplace(paramsHash, maxY * (std::fabs (dB) / maxDb));
		}
		else
		{
			auto const logMax = std::log (maxDb);
			auto const mappedDb = std::log (std::fabs (dB)) / logMax;
			yPosCache.emplace(paramsHash, maxY * mappedDb);
		}
	}
	return yPosCache.at(paramsHash);
}
} // namespace gui
} // namespace pe