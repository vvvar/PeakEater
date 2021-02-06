/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
namespace AppColors
{

static const juce::Colour Paper   { 250, 250, 250 };
static const juce::Colour Paper2  { 241, 241, 230 };
static const juce::Colour Red     { 210, 14, 44 };
static const juce::Colour Navy    { 0, 29, 45 };
static const juce::Colour Blue    { 156, 174, 188 };
static const juce::Colour Yellow  { 234, 153, 39 };
static const juce::Colour Green   { 115, 173, 0 };
static const juce::Colour Green2  { 59, 120, 0 };
static const juce::Colour Pastele { 255, 235, 204 };
static const juce::Colour Grey    { 47, 72, 88 };

};

//==============================================================================
class MeterSlider : public juce::Component
{
public:
    //==============================================================================
    class CustomMeterLookAndFeel : public foleys::LevelMeterLookAndFeel
    {
    public:
        juce::Rectangle<float> getMeterTickmarksBounds (juce::Rectangle<float>, foleys::LevelMeter::MeterFlags) const override
        {
            return {};
        }
        juce::Rectangle<float> getMeterClipIndicatorBounds (juce::Rectangle<float>, foleys::LevelMeter::MeterFlags) const override
        {
            return {};
        }
        juce::Rectangle<float> getMeterMaxNumberBounds (juce::Rectangle<float>, foleys::LevelMeter::MeterFlags) const override
        {
            return {};
        }
        void drawMeterBarBackground (juce::Graphics& g,
                                     foleys::LevelMeter::MeterFlags meterType,
                                     juce::Rectangle<float> bounds) override
        {
            juce::ignoreUnused (meterType);
            g.setColour (findColour (foleys::LevelMeter::lmMeterBackgroundColour));
            g.fillRoundedRectangle  (bounds, ROUNDING);

            g.setColour (findColour (foleys::LevelMeter::lmMeterOutlineColour));
            g.drawRoundedRectangle (bounds, ROUNDING, 1.0);
        }
        void drawMeterBar (juce::Graphics& g,
                           foleys::LevelMeter::MeterFlags meterType,
                           juce::Rectangle<float> bounds,
                           float rms, float peak) override
        {
            const auto infinity = meterType & foleys::LevelMeter::Reduction ? -30.0f :  -100.0f;
            const auto rmsDb  = juce::Decibels::gainToDecibels (rms,  infinity);
            const auto peakDb = juce::Decibels::gainToDecibels (peak, infinity);

            const juce::Rectangle<float> floored (ceilf (bounds.getX()), ceilf (bounds.getY()),
                                                  floorf (bounds.getRight()) - (ceilf (bounds.getX())),
                                                  floorf (bounds.getBottom()) - (ceilf (bounds.getY())));

            if (meterType & foleys::LevelMeter::Vintage) {
                // TODO
            }
            else if (meterType & foleys::LevelMeter::Reduction)
            {
                const float limitDb = juce::Decibels::gainToDecibels (rms, infinity);
                g.setColour (findColour (foleys::LevelMeter::lmMeterReductionColour));
                if (meterType & foleys::LevelMeter::Horizontal)
                    g.fillRect (floored.withLeft (floored.getX() + limitDb * floored.getWidth() / infinity));
                else
                    g.fillRect (floored.withBottom (floored.getY() + limitDb * floored.getHeight() / infinity));
            }
            else
            {
                if (meterType & foleys::LevelMeter::Horizontal)
                {
                    if (horizontalGradient.getNumColours() < 2)
                    {
                        horizontalGradient = juce::ColourGradient (findColour (foleys::LevelMeter::lmMeterGradientLowColour),
                                                                   floored.getX(), floored.getY(),
                                                                   findColour (foleys::LevelMeter::lmMeterGradientMaxColour),
                                                                   floored.getRight(), floored.getY(), false);
                        horizontalGradient.addColour (0.5, findColour (foleys::LevelMeter::lmMeterGradientLowColour));
                        horizontalGradient.addColour (0.75, findColour (foleys::LevelMeter::lmMeterGradientMidColour));
                    }
                    g.setGradientFill (horizontalGradient);
                    g.fillRoundedRectangle (floored.withRight (floored.getRight() - rmsDb * floored.getWidth() / infinity), ROUNDING);

                    if (peakDb > -49.0)
                    {
                        g.setColour (findColour ((peakDb > -0.3f) ? foleys::LevelMeter::lmMeterMaxOverColour :
                                                 ((peakDb > -5.0) ? foleys::LevelMeter::lmMeterMaxWarnColour :
                                                  foleys::LevelMeter::lmMeterMaxNormalColour)));
                        g.drawVerticalLine (juce::roundToInt (floored.getRight() - juce::jmax (peakDb * floored.getWidth() / infinity, 0.0f)),
                                            floored.getY(), floored.getBottom());
                    }
                }
                else
                {
                    // vertical
                    if (verticalGradient.getNumColours() < 2)
                    {
                        verticalGradient = juce::ColourGradient (findColour (foleys::LevelMeter::lmMeterGradientLowColour),
                                                                 floored.getX(), floored.getBottom(),
                                                                 findColour (foleys::LevelMeter::lmMeterGradientMaxColour),
                                                                 floored.getX(), floored.getY(), false);
                        verticalGradient.addColour (0.5f, findColour (foleys::LevelMeter::lmMeterGradientLowColour));
                        verticalGradient.addColour (0.75f, findColour (foleys::LevelMeter::lmMeterGradientMidColour));
                    }
                    g.setGradientFill (verticalGradient);
                    g.fillRect (floored.withTop (floored.getY() + rmsDb * floored.getHeight() / infinity));

                    if (peakDb > -49.0f) {
                        g.setColour (findColour ((peakDb > -0.3f) ? foleys::LevelMeter::lmMeterMaxOverColour :
                                                 ((peakDb > -5.0f) ? foleys::LevelMeter::lmMeterMaxWarnColour :
                                                  foleys::LevelMeter::lmMeterMaxNormalColour)));
                        g.drawHorizontalLine (juce::roundToInt (floored.getY() + juce::jmax (peakDb * floored.getHeight() / infinity, 0.0f)),
                                              floored.getX(), floored.getRight());
                    }
                }
            }
        }
    private:
        const float ROUNDING = 3.0f;
    };
    
    MeterSlider(
                foleys::LevelMeterSource& meterSource,
                juce::AudioProcessorValueTreeState& valueTreeState,
                const Parameters::ParameterInfo& parameter
                )
    {
        lnf.setColour (foleys::LevelMeter::lmBackgroundColour, AppColors::Paper);
        lnf.setColour (foleys::LevelMeter::lmMeterGradientLowColour, AppColors::Green);
        lnf.setColour (foleys::LevelMeter::lmMeterGradientMidColour, AppColors::Green2);
        lnf.setColour (foleys::LevelMeter::lmMeterGradientMaxColour, AppColors::Red);
        lnf.setColour (foleys::LevelMeter::lmMeterBackgroundColour, AppColors::Blue);
        lnf.setColour (foleys::LevelMeter::lmMeterOutlineColour, AppColors::Blue);
        
        meter.setLookAndFeel (&lnf);
        meter.setFixedNumChannels (1);
        meter.setMeterSource (&meterSource);
        addAndMakeVisible (meter);
        
        slider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
        slider.setColour (juce::Slider::trackColourId, AppColors::Navy.withAlpha(0.0f));
        slider.setColour (juce::Slider::backgroundColourId, AppColors::Blue.withAlpha(0.0f));
        
        addAndMakeVisible (slider);
        attachment.reset (new SliderAttachment (valueTreeState, parameter.Id, slider));
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        meter.setBounds (-52, 26, getWidth() - 33, getHeight() - 46);
        slider.setBounds (getLocalBounds());
    }
    
    void setEnabled(bool isEnabled)
    {
        slider.setEnabled(isEnabled);
        meter.setEnabled(isEnabled);
    }
private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    juce::Slider slider { juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight };
    std::unique_ptr<SliderAttachment> attachment;
    
    CustomMeterLookAndFeel lnf;
    foleys::LevelMeter meter { foleys::LevelMeter::Horizontal };
};

//==============================================================================
class PeakAnalyzer : public juce::Component, public juce::Timer
{
public:
    //==============================================================================
    PeakAnalyzer(MultiShaperAudioProcessor& p):
        processor(p)
    {
        addAndMakeVisible(inputMagnitudeLabel);
        addAndMakeVisible(inputMagnitudeValue);
        addAndMakeVisible(clippedMagnitudeLabel);
        addAndMakeVisible(clippedMagnitudeValue);
        addAndMakeVisible(outputMagnitudeLabel);
        addAndMakeVisible(outputMagnitudeValue);
        startTimer(1000);
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        juce::Grid grid;
         
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        
        grid.alignContent = juce::Grid::AlignContent::start;
        
        grid.templateRows = {
            Track (Fr (1))
        };
        
        grid.templateColumns = {
            Track (Fr (1)), Track (Fr (2)),
            Track (Fr (1)), Track (Fr (2)),
            Track (Fr (1)), Track (Fr (2))
        };
        
        grid.items = {
            Item (inputMagnitudeLabel),   Item (Item(inputMagnitudeValue)),
            Item (outputMagnitudeLabel),  Item (outputMagnitudeValue),
            Item (clippedMagnitudeLabel), Item (clippedMagnitudeValue)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    //==============================================================================
    void timerCallback() override
    {
        inputMagnitudeValue.setText (dbToString (processor.getDecibelsIn()), juce::dontSendNotification);
        outputMagnitudeValue.setText (dbToString (processor.getDecibelsOut()), juce::dontSendNotification);
        clippedMagnitudeValue.setText (dbToString (processor.getDecibelsClipped()), juce::dontSendNotification);
    }
private:
    //==============================================================================
    MultiShaperAudioProcessor& processor;
    
    //==============================================================================
    juce::Label inputMagnitudeLabel { "InputMagnitudeLabel", "In:" };
    juce::Label inputMagnitudeValue { "InputMagnitudeValue", "0 dB" };
    
    juce::Label clippedMagnitudeLabel { "ClippedMagnitudeLabel", "Eaten:" };
    juce::Label clippedMagnitudeValue { "ClippedMagnitudeValue", "0 dB" };
    
    juce::Label outputMagnitudeLabel { "OutputMagnitudeLabel", "Out:" };
    juce::Label outputMagnitudeValue { "OutputMagnitudeValue", "0 dB" };
    
    //==============================================================================
    juce::String dbToString(const float db) const noexcept
    {
        return juce::String (std::roundf (db * 100) / 100) + " dB";
    }
};

//==============================================================================
class CheckBox : public juce::Component
{
public:
    //==============================================================================
    CheckBox(const Parameters::ParameterInfo& parameter, juce::AudioProcessorValueTreeState& vts) noexcept
    {
        label.setText (parameter.Label, juce::dontSendNotification);
        addAndMakeVisible (label);
        addAndMakeVisible (toggle);
        attachment.reset (new ButtonAttachment (vts, parameter.Id, toggle));
    }
    
    //==============================================================================
    void paint (juce::Graphics&) override
    {}
    
    void resized() override
    {
        juce::Grid grid;
         
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        
        grid.templateRows = {
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (1)) };

        grid.items = {
            Item (label), Item (toggle)
        };
         
        grid.performLayout (getLocalBounds());
    }
    
    void addListener(juce::Button::Listener* listener)
    {
        toggle.addListener(listener);
    }
private:
    //==============================================================================
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    //==============================================================================
    juce::Label                       label;
    juce::ToggleButton                toggle;
    std::unique_ptr<ButtonAttachment> attachment;
};

//==============================================================================
class MainWindow : public juce::Component, public juce::Button::Listener
{
public:
    //==============================================================================
    MainWindow(MultiShaperAudioProcessor& p, juce::AudioProcessorValueTreeState& vts):
        ceilingSlider(p.getCeilingMeterSource(), vts, Parameters::Ceiling),
        linkInOut(Parameters::LinkInOut, vts),
        peaksEaten(p)
    {
        addAndMakeVisible (logo);
        addAndMakeVisible(logo_placeholder);
        juce::Image logoImage = juce::ImageCache::getFromMemory (BinaryData::logo_full_png, BinaryData::logo_full_pngSize);
        logo.setImages(false, true, true, logoImage, 1.0f, {}, logoImage, 1.0f, {}, logoImage, 1.0f, {});
        
        inputGainLabel.setText (Parameters::InputGain.Label, juce::dontSendNotification);
        addAndMakeVisible (inputGainLabel);
        addAndMakeVisible (inputGainSlider);
        inputGainAttachment.reset (new SliderAttachment (vts, Parameters::InputGain.Id, inputGainSlider));
        
        outputGainLabel.setText (Parameters::OutputGain.Label, juce::dontSendNotification);
        addAndMakeVisible (outputGainLabel);
        addAndMakeVisible (outputGainSlider);
        outputGainAttachment.reset (new SliderAttachment (vts, Parameters::OutputGain.Id, outputGainSlider));
        
        ceilingLabel.setText (Parameters::Ceiling.Label, juce::dontSendNotification);
        addAndMakeVisible (ceilingLabel);
        addAndMakeVisible (ceilingSlider);
        
        clippingTypeLabel.setText (Parameters::ClippingType.Label, juce::dontSendNotification);
        addAndMakeVisible (clippingTypeLabel);
        addAndMakeVisible (clippingTypeSlider);
        clippingTypeAttachment.reset (new SliderAttachment (vts, Parameters::ClippingType.Id, clippingTypeSlider));
        
        oversampleRateLabel.setText (Parameters::OversampleRate.Label, juce::dontSendNotification);
        addAndMakeVisible (oversampleRateLabel);
        addAndMakeVisible (oversampleRateSlider);
        oversampleRateAttachment.reset (new SliderAttachment (vts, Parameters::OversampleRate.Id, oversampleRateSlider));
        
        getLookAndFeel().setColour (juce::Slider::thumbColourId, AppColors::Red);
        getLookAndFeel().setColour (juce::Slider::trackColourId, AppColors::Navy);
        getLookAndFeel().setColour (juce::Slider::backgroundColourId, AppColors::Blue);
        getLookAndFeel().setColour (juce::Slider::textBoxOutlineColourId, AppColors::Blue);
        getLookAndFeel().setColour (juce::Slider::textBoxBackgroundColourId, AppColors::Paper);
        getLookAndFeel().setColour (juce::Slider::textBoxHighlightColourId, AppColors::Navy);
        getLookAndFeel().setColour (juce::ToggleButton::tickColourId, AppColors::Navy);
        getLookAndFeel().setColour (juce::ToggleButton::tickDisabledColourId, AppColors::Navy);
        getLookAndFeel().setColour (juce::Label::textColourId, AppColors::Navy);
        
        inputGainSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
        outputGainSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
        ceilingSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
        clippingTypeSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
        oversampleRateSlider.setColour (juce::Slider::textBoxTextColourId, AppColors::Navy);
        
        addAndMakeVisible(linkInOut);
        linkInOut.addListener(this);
        
        addAndMakeVisible(peaksEaten);
    }
    
    //==============================================================================
    void buttonClicked (juce::Button *) override
    {}
    
    void buttonStateChanged (juce::Button* button) override
    {
        if (button->getToggleState())
        {
            outputGainSlider.setEnabled(false);
            outputGainLabel.setColour (juce::Label::textColourId, AppColors::Blue);
            outputGainSlider.setColour (juce::Slider::thumbColourId, AppColors::Blue);
            outputGainSlider.setColour (juce::Slider::trackColourId, AppColors::Blue);
        } else
        {
            outputGainSlider.setEnabled(true);
            outputGainLabel.setColour (juce::Label::textColourId, AppColors::Navy);
            outputGainSlider.setColour (juce::Slider::thumbColourId, AppColors::Red);
            outputGainSlider.setColour (juce::Slider::trackColourId, AppColors::Navy);
        }
    }
    
    void paint (juce::Graphics&) override
    {
        
    }
    
    void resized() override
    {
        juce::Grid grid;
         
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        
        grid.templateRows = {
            Track (Fr (1)),
            Track (Fr (1)),
            Track (Fr (1)),
            Track (Fr (1)),
            Track (Fr (1)),
            Track (Fr (1)),
            Track (Fr (1))
        };
        grid.templateColumns = { Track (Fr (1)), Track (Fr (5)) };
        
        grid.items = {
            Item (logo_placeholder), Item (logo).withMargin(juce::GridItem::Margin (5, 0, 0, 170)),
            Item (inputGainLabel), Item (inputGainSlider),
            Item (outputGainLabel), Item (outputGainSlider),
            Item (ceilingLabel), Item (ceilingSlider),
            Item (clippingTypeLabel), Item (clippingTypeSlider),
            Item (oversampleRateLabel), Item (oversampleRateSlider),
            Item (linkInOut), Item (peaksEaten),
        };
         
        grid.performLayout (getLocalBounds());
    }
    
private:
    //==============================================================================
    using SliderAttachment   = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    //==============================================================================
    juce::ImageButton logo;
    juce::ImageButton logo_placeholder;
    
    juce::Label inputGainLabel;
    juce::Slider inputGainSlider { juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight };
    std::unique_ptr<SliderAttachment> inputGainAttachment;
    
    juce::Label outputGainLabel;
    juce::Slider outputGainSlider { juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight };
    std::unique_ptr<SliderAttachment> outputGainAttachment;
    
    juce::Label ceilingLabel;
    MeterSlider ceilingSlider;
    
    juce::Label  clippingTypeLabel;
    juce::Slider clippingTypeSlider { juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight };
    std::unique_ptr<SliderAttachment> clippingTypeAttachment;
    
    juce::Label  oversampleRateLabel;
    juce::Slider oversampleRateSlider { juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight };
    std::unique_ptr<SliderAttachment> oversampleRateAttachment;
    
    CheckBox linkInOut;
    
    PeakAnalyzer peaksEaten;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

//==============================================================================
class MultiShaperAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MultiShaperAudioProcessorEditor (MultiShaperAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~MultiShaperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    MultiShaperAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;
    
    MainWindow main;
    
    foleys::LevelMeterLookAndFeel lnf;
    foleys::LevelMeter inputMeter  { foleys::LevelMeter::Default };
    foleys::LevelMeter outputMeter { foleys::LevelMeter::Default };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiShaperAudioProcessorEditor)
};
