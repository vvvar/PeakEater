#include "PeakMeter.h"

namespace pe
{
namespace gui
{
PeakMeter::PeakMeter (int const& sampleRate, float const& minValue)
	: mMinValue (minValue)
	, mBufferSize (sampleRate * 2) // 2 seconds of look behind
{
	reset();
}

void PeakMeter::push (float const& nextValue)
{
	mBuffer.pop_front();
	mBuffer.push_back (nextValue);
}

float PeakMeter::getMagnitude()
{
	return *std::max_element (mBuffer.begin(), mBuffer.end());
}

void PeakMeter::reset()
{
	mBuffer.clear();
	for (int x = 0; x < mBufferSize; x++)
	{
		mBuffer.push_back (mMinValue);
	}
}
} // namespace gui
} // namespace pe