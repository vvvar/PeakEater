#include "PeakMeter.h"

#include <algorithm>

namespace pe::gui {

namespace {
auto constexpr gLookBehindSeconds = 3;
}

PeakMeter::PeakMeter(int const& sampleRate, float const& minValue)
    : mMinValue(minValue),
      // 4 seconds of look behind
      mBufferSize(sampleRate * gLookBehindSeconds) {
    reset();
}

void PeakMeter::push(float const& nextValue) {
    mBuffer.pop_front();
    mBuffer.push_back(nextValue);
}

float PeakMeter::getMaxPeak() { return *std::ranges::max_element(mBuffer); }

void PeakMeter::reset() {
    mBuffer.clear();
    for (int x = 0; x < mBufferSize; x++) {
        mBuffer.push_back(mMinValue);
    }
}
}  // namespace pe::gui
