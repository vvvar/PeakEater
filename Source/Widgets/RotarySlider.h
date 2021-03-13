#pragma once

#include <JuceHeader.h>

#include "../AppColours.h"
#include "../AppTheme.h"
#include "../Parameters.h"

namespace widgets
{

class RotarySlider : public juce::Slider
{
public:
    //==============================================================================
    class SliderLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        SliderLookAndFeel()
        {
            setColour (juce::Slider::textBoxTextColourId, AppTheme::V1::RSliderFontColour);
            setColour (juce::Slider::textBoxOutlineColourId, AppTheme::V1::RSliderFontColour.withAlpha (0.0f));
        }
        ~SliderLookAndFeel() {}
        juce::Font getLabelFont (juce::Label& label) override
        {
            label.setJustificationType (juce::Justification::centred);
            return { AppTheme::V1::RSliderFontSize };
        }
        void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                               const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
        {
            auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
            auto centreX = (float) x + (float) width  * 0.5f;
            auto centreY = (float) y + (float) height * 0.5f;
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
            
            // fill
            g.setColour (AppTheme::V1::RSliderBackgroundColour);
            g.fillEllipse (rx, ry, rw, rw);
     
            // outline
            g.setColour (AppTheme::V1::RSliderBorderColour);
            g.drawEllipse (rx, ry, rw, rw, 1.0f);
            
            juce::Path p;
            auto pointerLength = radius * 0.33f;
            auto pointerThickness = 2.0f;
            p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
            p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
            
            // pointer
            g.setColour (AppTheme::V1::RSliderPointerColour);
            g.fillPath (p);
            
            // Value
            g.setColour (slider.findColour (Slider::textBoxTextColourId));
            g.setFont (radius * 0.4f);
            float value = floorf(slider.getValue() * 100) / 100;
            g.drawSingleLineText (juce::String (value) + " dB", centreX, centreY * 1.04f, juce::Justification::horizontallyCentred);
        }
    };
    
    //==============================================================================
    RotarySlider(
           const Parameters::ParameterInfo& parameter,
           juce::AudioProcessorValueTreeState& vts,
           juce::String tooltipText = "")
    : juce::Slider(parameter.Id)
    {
        setSliderStyle (juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        setTooltip (tooltipText);
        setLookAndFeel (&lnf);
        attachment.reset (new SliderAttachment (vts, parameter.Id, *this));
    }
    
    ~RotarySlider() override
    {
        setLookAndFeel(nullptr);
    }
    
    void setEnabled(bool isEnabled)
    {
        juce::Slider::setEnabled(isEnabled);
        if (isEnabled)
        {
            setColour (juce::Slider::thumbColourId, AppColors::Red);
            setColour (juce::Slider::trackColourId, AppColors::Navy);
        } else
        {
            setColour (juce::Slider::thumbColourId, AppColors::Blue);
            setColour (juce::Slider::trackColourId, AppColors::Blue);
        }
    }
    
private:
    //==============================================================================
    SliderLookAndFeel lnf;
    
    //==============================================================================
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> attachment;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RotarySlider)
};

}
