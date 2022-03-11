//Copyright © 2022 TIGan. All Rights Reserved.

#include "ContentBrowserMenuExtension.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "CustomThumbnails/CustomThumbnails.h"

DEFINE_LOG_CATEGORY(LogCTContentBrowserMenuExtension)

FContentBrowserMenuExtension::FContentBrowserMenuExtension(const TArray<FAssetData>& Assets)
    : AssetRegistry(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get()),
      CTModule(FModuleManager::LoadModuleChecked<FCustomThumbnailsModule>(TEXT("CustomThumbnails"))),
      SelectedAssets(Assets) {}

void FContentBrowserMenuExtension::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
    UE_LOG(LogCTContentBrowserMenuExtension, VeryVerbose, TEXT("AddMenuEntry Start"));

    MenuBuilder.AddMenuEntry(                                                                                 //
        FText::FromString("Add Custom Thumbnail"),                                                            //
        FText::FromString("Add a thumbnail for this asset and all its children"),                             //
        FSlateIcon("CustomThumbnailsStyle", "ContentBrowser.AssetAction.AddThumbnail"),                       //
        FUIAction(FExecuteAction::CreateSP(this, &FContentBrowserMenuExtension::OnAddThumbnailButtonClicked)) //
        );

    UE_LOG(LogCTContentBrowserMenuExtension, VeryVerbose, TEXT("AddMenuEntry End"));
}

void FContentBrowserMenuExtension::OnAddThumbnailButtonClicked()
{
    UE_LOG(LogCTContentBrowserMenuExtension, VeryVerbose, TEXT("OnButtonClicked"));

    FGlobalTabmanager::Get()->TryInvokeTab(CTModule.GetThumbnailPickerTabName());
    return;
}
