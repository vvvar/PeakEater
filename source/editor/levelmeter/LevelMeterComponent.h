#pragma once

#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../Ticks.h"
#include "processor/LevelMeter.h"

namespace pe {
namespace gui {
class LevelMeterComponent : public juce::Component {
   public:
    LevelMeterComponent(std::shared_ptr<processor::LevelMeter<float>> levelMeter, std::shared_ptr<Ticks> ticks, std::string const& name);
    ~LevelMeterComponent() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void drawTicks(std::vector<float> const& ticksLevels, juce::Colour const&& colour, juce::Graphics& g);
    void drawTicks(std::vector<float> const& ticksLevels, juce::Colour const& colour, juce::Graphics& g);

   private:
    class LevelMeterTimer : public juce::Timer {
       public:
        LevelMeterTimer(std::function<void()> callback) : mCallback(callback) {}

        void timerCallback() override {
            if (mCallback) {
                mCallback();
            }
        }

       private:
        std::function<void()> mCallback;
    };

    std::string mName;
    LevelMeterTimer mMeterTimer;
    std::shared_ptr<Ticks> mTicks;
    std::shared_ptr<processor::LevelMeter<float>> mLevelMeter;
    juce::NormalisableRange<float> mRange;
    juce::LinearSmoothedValue<float> mSmoothedGain;
    juce::Label mLabel;

    void onTimerTick();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterComponent)
};
}  // namespace gui
}  // namespace pe