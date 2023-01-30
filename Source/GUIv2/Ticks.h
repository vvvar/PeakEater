#pragma once

#include <vector>
#include <atomic>

namespace pe
{
namespace gui
{
class Ticks
{
public:
std::vector<float>& getTicksList()
{
	return mTicksLists.at (mCurrentTicksList);
}

void switchToNextTicksList()
{
	if (mCurrentTicksList >= mTicksLists.size() - 1)
	{
		mCurrentTicksList = 0;
	}
	else
	{
		mCurrentTicksList++;
	}
}

private:
std::atomic<unsigned int> mCurrentTicksList{ 0 };
std::vector<std::vector<float> > mTicksLists{
	{ 0.0f, -5.0f, -8.0f, -11.0f, -14.0f, -17.0f, -20.0f, -23.0f, -26.0f, -29.0f, -32.0f, -36.0f },
	{ 0.0f, -3.0f, -5.0f, -8.0f, -11.0f, -14.0f, -17.0f, -20.0f, -23.0f, -26.0f, -29.0f, -32.0f, -36.0f },
	{ 0.0f, -3.0f, -6.0f, -9.0f, -12.0f, -15.0f, -18.0f, -21.0f, -24.0f, -27.0f, -30.0f, -33.0f, -36.0f },
	{ 0.0f, -2.0f, -4.0f, -6.0f, -8.0f, -10.0f, -12.0f, -14.0f, -16.0f, -18.0f, -20.0f, -22.0f, -24.0f, -27.0f, -36.0f },
	{ 0.0f, -4.0f, -6.0f, -8.0f, -12.0f, -14.0f, -16.0f, -18.0f, -20.0f, -22.0f, -24.0f, -26.0f, -28.0f, -30.0f, -32.0f, -34.0f, -36.0f },
	{ 0.0f, -3.0f, -6.0f, -9.0f, -12.0f, -15.0f, -24.0f, -36.0f },
};
};
} // namespace gui
} // namespace pe