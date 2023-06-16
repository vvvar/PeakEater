#include "EditableParameterComponent.h"
#include "../Utils.h"
#include <juce_graphics/juce_graphics.h>

namespace pe
{
namespace gui
{
    EditableParameterComponent::EditableParameterComponent (
        std::shared_ptr<juce::AudioProcessorValueTreeState> parameters,
        std::string const& parameterId)
        : mValueAttachment (*parameters->getParameter (parameterId), mValueLabel)
    {
        mValueLabel.setEditable (true);
        mValueLabel.setBorderSize (juce::BorderSize<int> (0));
        mValueLabel.onEditorShow = std::bind (&EditableParameterComponent::resized, this);
        mValueLabel.onEditorHide = std::bind (&EditableParameterComponent::resized, this);
        mValueLabel.setLookAndFeel (&mValueLabelLookAndFeel);
        addAndMakeVisible (mValueLabel);

        mSuffixLabel.setBorderSize (juce::BorderSize<int> (0));
        mSuffixLabel.addMouseListener (this, false);
        addAndMakeVisible (mSuffixLabel);

        updateFontSize();
    }

    EditableParameterComponent::~EditableParameterComponent()
    {
        mSuffixLabel.removeMouseListener (this);
        mValueLabel.setLookAndFeel (nullptr);
        mSuffixLabel.setLookAndFeel (nullptr);
        setLookAndFeel (nullptr);
    }

    void EditableParameterComponent::resized()
    {
        juce::Grid grid;
        using Track = juce::Grid::TrackInfo;
        using Fr = juce::Grid::Fr;
        using Item = juce::GridItem;
        grid.templateRows = { Track (Fr (1)) };
        if (mValueLabel.isBeingEdited())
        {
            grid.templateColumns = { Track (Fr (1)) };
            grid.items = { Item (mValueLabel) };
            mValueLabel.setJustificationType (juce::Justification::centred);
            mSuffixLabel.setVisible (false);
        }
        else if (mSuffixLabel.getText().isNotEmpty())
        {
            grid.templateColumns = { Track (Fr (1)), Track (Fr (1)) };
            grid.items = { Item (mValueLabel), Item (mSuffixLabel) };
            mValueLabel.setJustificationType (juce::Justification::right);
            mSuffixLabel.setJustificationType (juce::Justification::left);
            mSuffixLabel.setVisible (true);
        }
        else
        {
            grid.templateColumns = { Track (Fr (1)) };
            grid.items = { Item (mValueLabel) };
            mValueLabel.setJustificationType (juce::Justification::centred);
            mSuffixLabel.setVisible (false);
        }
        grid.performLayout (getLocalBounds());
        updateFontSize();
    }

    void EditableParameterComponent::setSuffix (std::string const& suffix)
    {
        mSuffixLabel.setText (suffix, juce::NotificationType::dontSendNotification);
    }

    void EditableParameterComponent::updateFontSize()
    {
        auto const topLevelBounds = getTopLevelComponent()->getBounds();
        auto const topLevelWidth = topLevelBounds.getWidth();
        auto const topLevelHeight = topLevelBounds.getHeight();
        auto const fontSize = calculateTextSize (topLevelWidth, topLevelHeight);
        mValueLabel.setFont (fontSize);
        mSuffixLabel.setFont (fontSize);
        if (auto editor = mValueLabel.getCurrentTextEditor())
        {
            editor->setJustification (juce::Justification::centred);
            editor->setBorder (juce::BorderSize<int> (0));
        }
    }

    void EditableParameterComponent::mouseDown (juce::MouseEvent const&)
    {
        if (! mValueLabel.isBeingEdited())
        {
            mValueLabel.showEditor();
        }
    }
} // namespace gui
} // namespace pe