#pragma once

#include <deque>

namespace pe::gui {
class PeakMeter {
   public:
    PeakMeter(int const& sampleRate, float const& minValue);
    void push(float const& nextValue);
    float getMaxPeak();
    void reset();

   private:
    float mMinValue;
    int const mBufferSize;
    std::deque<float> mBuffer;
};
}  // namespace pe::gui
