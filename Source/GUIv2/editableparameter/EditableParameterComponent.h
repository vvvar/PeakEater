#pragma once

#include "EditableLabelAttachment.h"
#include "EditableParameterComponentLookAndFeel.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
class EditableParameterComponent : public juce::Component
{
public:
EditableParameterComponent (
	std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
	std::string const& parameterId);
~EditableParameterComponent() override;

void resized() override;
void setSuffix (std::string const& suffix);
void mouseDown (juce::MouseEvent const&) override;

private:
juce::Label mValueLabel;
juce::Label mSuffixLabel;
EditableParameterComponentLookAndFeel mValueLabelLookAndFeel;
EditableLabelAttachment mValueAttachment;

void updateFontSize();

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditableParameterComponent)
};
} // namespace gui
} // namespace pe