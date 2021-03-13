#pragma once

#include <JuceHeader.h>
#include "AppColours.h"

//==============================================================================
namespace AppTheme
{

namespace V1
{

// Globals
static const auto BackgroundColor = AppColors::V2::Jet;
static const auto BackgroundGradientFrom = AppColors::V2::DavysGrey;
static const auto BackgroundGradientTo = AppColors::V2::Jet;
static const auto TextColor = AppColors::V2::Powder;

// Panel Block
static const auto PanelBackgroundColor = AppColors::V2::Jet;
static const auto PanelNameTextSize = 18.0f;
static const auto PanelBorderColor = AppColors::V2::DavysGrey;
static const auto PanelBorderRadius = 13.0f;
static const auto PanelBorderThickness = 1.0f;

// Tooltips
static const auto TooltipBackgroundColor = BackgroundColor;
static const auto TooltipTextColor = TextColor;
static const auto TooltipOutlineColor = PanelBackgroundColor;

// Meter Bar
static const auto MeterBackgroundColor = AppColors::V2::Transparent;
static const auto MeterLevelLowColor = AppColors::V2::TiffanyBlue;
static const auto MeterLevelMidColor = AppColors::V2::TiffanyBlue;
static const auto MeterLevelHighColor = AppColors::V2::RedRYB;
static const auto MeterLevelNormalText = MeterLevelMidColor;
static const auto MeterLevelClippedText = MeterLevelHighColor;
static const auto MeterTicksTextColor = TextColor;
static const auto MeterBarBackgroundColor = AppColors::V2::DavysGrey;

// Rotary Slider
static const auto RSliderBackgroundColour = AppColors::V2::LightGrey;
static const auto RSliderBorderColour = AppColors::V2::Jet;
static const auto RSliderPointerColour = AppColors::V2::Jet;
static const auto RSliderFontSize = 13.0f;
static const auto RSliderFontColour = AppColors::V2::Jet;

}



}
