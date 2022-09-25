#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
class EditableLabelAttachment : public juce::ParameterAttachment
{
public:
    EditableLabelAttachment (juce::RangedAudioParameter& parameter, juce::Label& labelComponent);
    ~EditableLabelAttachment() override;

private:
    juce::RangedAudioParameter& mParameter;
    juce::Label& mLabelComponent;

    void onParameterChanged (float newValue);
    void onLabelTextChanged();
};
} // namespace gui
} // namespace pe