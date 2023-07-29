#pragma once

#include "shared/LevelMeter.h"

namespace pe::gui {
struct LevelMetersPack {
    std::shared_ptr<pe::dsp::LevelMeter<float>> inputLevelMeter;
    std::shared_ptr<pe::dsp::LevelMeter<float>> clippingLevelMeter;
    std::shared_ptr<pe::dsp::LevelMeter<float>> outputLevelMeter;
};
}  // namespace pe::gui
