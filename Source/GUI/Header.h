#pragma once

#include <BinaryData.h>
#include <JuceHeader.h>

namespace pe
{
namespace gui
{
namespace layout
{
//==============================================================================
class Header : public juce::Component,
               public juce::Button::Listener
{
    class AlertContent : public juce::Component
    {
    public:
        AlertContent()
        {
            mPluginName.setText (ProjectInfo::projectName + juce::String{ " by " } + ProjectInfo::companyName, dontSendNotification);
            mPluginVersion.setText (juce::String{ "Version: " } + ProjectInfo::versionString, dontSendNotification);
            mGitHubPage.setButtonText (githubUrl.toString (false));
            mGitHubPage.setURL (githubUrl);
            mGitHubPage.setFont (mPluginVersion.getFont(), false);
            addAndMakeVisible (mPluginName);
            addAndMakeVisible (mPluginVersion);
            addAndMakeVisible (mGitHubPage);
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
                Track (Fr (1))
            };
            grid.templateColumns = { Track (Fr (1)) };

            grid.items = {
                Item (mPluginName),
                Item (mPluginVersion),
                Item (mGitHubPage)
            };

            grid.performLayout (getLocalBounds());
        }

    private:
        juce::Label mPluginName;
        juce::Label mPluginVersion;
        juce::HyperlinkButton mGitHubPage;
        juce::URL githubUrl{ "https://github.com/vvvar/PeakEater" };
    };

public:
    //==============================================================================
    Header()
    {
        juce::Image logoImage = juce::ImageCache::getFromMemory (BinaryData::logo_plugin_png, BinaryData::logo_plugin_pngSize);
        logo.setImages (false, true, true, logoImage, 1.0f, {}, logoImage, 1.0f, {}, logoImage, 1.0f, {});
        logo.setMouseCursor (juce::MouseCursor::PointingHandCursor);
        logo.addListener (this);
        addAndMakeVisible (logo);
    }

    //==============================================================================
    void paint (juce::Graphics&) override
    {
    }

    void resized() override
    {
        using Grid = juce::Grid;
        using Track = Grid::TrackInfo;
        using Fr = Grid::Fr;
        using Item = juce::GridItem;

        Grid grid;

        grid.templateRows = { Track (Fr (1)) };
        grid.templateColumns = { Track (Fr (1)) };
        grid.items = { Item (logo).withMargin (Item::Margin (0, 27, 0, 27)) };

        grid.performLayout (getLocalBounds());
    }

    void buttonClicked (juce::Button*) override
    {
        juce::DialogWindow::LaunchOptions launchOptions;
        launchOptions.dialogTitle = ProjectInfo::projectName + juce::String{ " - " } + ProjectInfo::versionString + " - Info";
        launchOptions.escapeKeyTriggersCloseButton = true;
        launchOptions.resizable = false;
        launchOptions.content.setOwned (new AlertContent);
        launchOptions.launchAsync()->centreWithSize (230, 100);
    }

private:
    //==============================================================================
    juce::ImageButton logo;

    //==============================================================================

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Header)
};
} // namespace layout
} // namespace gui
} // namespace pe
