#pragma once

#include <deque>

namespace pe
{
namespace gui
{
    class PeakMeter
    {
    public:
        PeakMeter (int const& sampleRate, float const& minValue);
        void push (float const& nextValue);
        float getMagnitude();
        void reset();

    private:
        float mMinValue;
        int const mBufferSize;
        std::deque<float> mBuffer;
    };
} // namespace gui
} // namespace pe