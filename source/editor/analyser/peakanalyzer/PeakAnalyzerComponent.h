#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <algorithm>
#include <atomic>
#include <numeric>

#include "../../LevelMetersPack.h"
#include "PeakMeter.h"

namespace pe {
namespace gui {
class PeakAnalyzerComponent : public juce::Component {
   public:
    PeakAnalyzerComponent(LevelMetersPack const& levelMetersPack);
    ~PeakAnalyzerComponent() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
    void mouseDown(juce::MouseEvent const& event) override;

   private:
    class UpdateTimer : public juce::Timer {
       public:
        UpdateTimer(std::function<void()> callback) : mCallback(callback) {}

        void timerCallback() override {
            if (mCallback) {
                mCallback();
            }
        }

       private:
        std::function<void()> mCallback;
    };

    std::shared_ptr<processor::LevelMeter<float>> mInputLevelMeter;
    std::shared_ptr<processor::LevelMeter<float>> mClippingLevelMeter;
    std::shared_ptr<processor::LevelMeter<float>> mOutputLevelMeter;

    PeakMeter mInputPeakMeter;
    PeakMeter mOutputPeakMeter;
    PeakMeter mEatenPeakMeter;
    UpdateTimer mUpdateTimer;
    juce::Label mInputLabel;
    juce::Label mOutputLabel;
    juce::Label mEatenLabel;

    void onUpdateTick();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PeakAnalyzerComponent)
};
}  // namespace gui
}  // namespace pe