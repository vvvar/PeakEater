#pragma once

#include <JuceHeader.h>
#include "../Widgets/BlockPanel.h"

namespace layout
{

class Clipper : public widgets::BlockPanel
{
public:
    Clipper(): widgets::BlockPanel("CLIPPER")
    {}
};

}
