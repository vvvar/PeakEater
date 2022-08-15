#pragma once

#include "DialLookAndFeel.h"
#include <juce_gui_basics/juce_gui_basics.h>

namespace pe
{
namespace gui
{
class Dial : public juce::Slider
{
public:
    Dial();
    ~Dial();

private:
    DialLookAndFeel mLookAndFeel;
};
} // namespace gui
} // namespace pe