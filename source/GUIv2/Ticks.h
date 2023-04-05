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

bool isLinear()
{
	return mIsLinear;
}

void setIsLinear(bool isLinear)
{
	mIsLinear = isLinear;
}

private:
std::atomic<bool> mIsLinear{true};
std::atomic<unsigned int> mCurrentTicksList{ 0 };
std::vector<std::vector<float> > mTicksLists{
	{ 0.0f, -4.0f, -8.0f, -12.0f, -16.0f, -20.0f, -25.0f, -30.0f, -36.0f },
	{ 0.0f, -3.0f, -6.0f, -10.0f, -16.0f, -24.0f, -36.0f },
	{ 0.0f, -3.0f, -5.0f, -8.0f, -11.0f, -15.0f, -20.0f, -25.0f, -32.0f, -36.0f },
};
};
} // namespace gui
} // namespace pe