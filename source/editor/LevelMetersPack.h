#pragma once

#include "processor/LevelMeter.h"

namespace pe::gui {
struct LevelMetersPack {
    std::shared_ptr<processor::LevelMeter<float>> inputLevelMeter;
    std::shared_ptr<processor::LevelMeter<float>> clippingLevelMeter;
    std::shared_ptr<processor::LevelMeter<float>> outputLevelMeter;
};
}  // namespace pe::gui
