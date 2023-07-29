#include "EditableLabelAttachment.h"

namespace pe
{
namespace gui
{
    EditableLabelAttachment::EditableLabelAttachment (juce::RangedAudioParameter& parameter, juce::Label& labelComponent)
        : juce::ParameterAttachment (parameter, std::bind (&EditableLabelAttachment::onParameterChanged, this, std::placeholders::_1)), mParameter (parameter), mLabelComponent (labelComponent)
    {
        mLabelComponent.onTextChange = std::bind (&EditableLabelAttachment::onLabelTextChanged, this);
        sendInitialUpdate();
    }

    EditableLabelAttachment::~EditableLabelAttachment()
    {
    }

    void EditableLabelAttachment::onParameterChanged (float newValue)
    {
        auto const normalised = mParameter.convertTo0to1 (newValue);
        mLabelComponent.setText (mParameter.getText (normalised, 0), juce::dontSendNotification);
    }

    void EditableLabelAttachment::onLabelTextChanged()
    {
        auto const labelText = mLabelComponent.getText();
        auto const denormalised = mParameter.convertFrom0to1 (mParameter.getValueForText (labelText));
        setValueAsCompleteGesture (denormalised);
    }
} // namespace gui
} // namespace pe