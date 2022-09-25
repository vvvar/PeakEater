#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
class EditableParameterComponentLookAndFeel : public juce::LookAndFeel_V4
{
    void drawTextEditorOutline (juce::Graphics&, int, int, juce::TextEditor&) override;
};
} // namespace gui
} // namespace pe