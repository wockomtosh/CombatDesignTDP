﻿#include "ENUpdatePopup.h"
#include "EditorStyleSet.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "SWebBrowser.h"
#include "Interfaces/IPluginManager.h"
#include "GenericPlatform/GenericPlatformMisc.h"

void ENUpdatePopup::OnBrowserLinkClicked(const FSlateHyperlinkRun::FMetadata& Metadata)
{
	const FString* URL = Metadata.Find(TEXT("href"));

	if (URL)
	{
		FPlatformProcess::LaunchURL(**URL, nullptr, nullptr);
	}
}

void ENUpdatePopup::Register()
{
    FString UpdateConfigPath = IPluginManager::Get().FindPlugin(TEXT("ElectronicNodes"))->GetBaseDir();
    UpdateConfigPath /= "UpdateConfig.ini";
    const FString UpdateConfigFile = FPaths::ConvertRelativePathToFull(FConfigCacheIni::NormalizeConfigIniPath(UpdateConfigPath));
    const FString CurrentPluginVersion = "3.15";

    FString LoadedConfig;
    if (FPaths::FileExists(UpdateConfigFile))
    {
        FFileHelper::LoadFileToString(LoadedConfig, *UpdateConfigFile);
    }
    else
    {
        LoadedConfig = TEXT("PluginVersionUpdate=");
        FFileHelper::SaveStringToFile(LoadedConfig, *UpdateConfigFile);
    }

    FString PluginVersionUpdate;
    FParse::Value(*LoadedConfig, TEXT("PluginVersionUpdate="), PluginVersionUpdate);

    if (PluginVersionUpdate != CurrentPluginVersion)
    {
        PluginVersionUpdate = CurrentPluginVersion;
        LoadedConfig = FString::Printf(TEXT("PluginVersionUpdate=%s"), *PluginVersionUpdate);
        FFileHelper::SaveStringToFile(LoadedConfig, *UpdateConfigFile);
    	
        FCoreDelegates::OnPostEngineInit.AddLambda([]()
        {
            Open();
        });
    }
}

void ENUpdatePopup::Open()
{
	if (!FSlateApplication::Get().CanDisplayWindows())
	{
		return;
	}

	TSharedRef<SBorder> WindowContent = SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("ToolPanel.GroupBorder"))
		.Padding(FMargin(8.0f, 8.0f));

	TSharedPtr<SWindow> Window = SNew(SWindow)
		.AutoCenter(EAutoCenter::PreferredWorkArea)
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.SizingRule(ESizingRule::FixedSize)
		.ClientSize(FVector2D(600, 400))
		.Title(FText::FromString("Electronic Nodes"))
		.IsTopmostWindow(true)
		[
			WindowContent
		];

	const FSlateFontInfo HeadingFont = FCoreStyle::GetDefaultFontStyle("Regular", 24);
	const FSlateFontInfo ContentFont = FCoreStyle::GetDefaultFontStyle("Regular", 12);

	TSharedRef<SVerticalBox> InnerContent = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(STextBlock)
			.Font(HeadingFont)
			.Text(FText::FromString("Electronic Nodes v3.15"))
		]
		+ SVerticalBox::Slot()
		.FillHeight(1.0)
		.Padding(10)
		[
			SNew(SBorder)
			.Padding(10)
			.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
			[
				SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SNew(SRichTextBlock)
					.Text(FText::FromString(R"(
<LargeText>Hello and thank you for using Electronic Nodes!</>

First thing first, if you've been enjoying using it, it would mean a lot if you could just drop <a id="browser" href="https://bit.ly/2U1YT8O">a small review on the marketplace page</> :). I also wanted to mention that I made another plugin to update the engine theme called <a id="browser" href="https://bit.ly/2TolSKQ">Darker Nodes</>.

I also made a marketplace search engine called <a id="browser" href="https://bit.ly/3uhO9CG">Orbital Market</>. It's completely free, super fast and full of filters to refine your search.

But let's keep it short, here are the cool new features (and bugfixes) of version 3.15!


<LargeText>Version 3.15</>

<RichTextBlock.Bold>Bugfixes</>

* Fix Popup appearing on each start (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/95">issue #95</>)


<LargeText>Version 3.14</>

<RichTextBlock.Bold>Features</>

* Remove annoying licence check (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/89">issue #89</>)

<RichTextBlock.Bold>Bugfixes</>

* Fix Metasound activation (<a id="browser" href="https://github.com/hugoattal/ElectronicNodes/issues/90">issue #90</>)


<a id="browser" href="https://github.com/hugoattal/ElectronicNodes#changelog">See complete changelog</>
)"))
					.TextStyle(FAppStyle::Get(), "NormalText")
					.DecoratorStyleSet(&FAppStyle::Get())
					.AutoWrapText(true)
					+ SRichTextBlock::HyperlinkDecorator(
						TEXT("browser"), FSlateHyperlinkRun::FOnClick::CreateStatic(&OnBrowserLinkClicked))
				]
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Leave a review <3"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([]()
				{
					const FString URL = "https://bit.ly/2RPhNPl";
					FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);

					return FReply::Handled();
				})
			]
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(20, 10))
			]
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Discover Darker Nodes"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([]()
				{
					const FString URL = "https://bit.ly/3vqUdGE";
					FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);

					return FReply::Handled();
				})
			]
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(20, 10))
			]
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SButton)
				.Text(FText::FromString("Close this window"))
				.HAlign(HAlign_Center)
				.OnClicked_Lambda([Window]()
				{
					Window->RequestDestroyWindow();

					return FReply::Handled();
				})
			]
		];

	WindowContent->SetContent(InnerContent);
	Window = FSlateApplication::Get().AddWindow(Window.ToSharedRef());
}
