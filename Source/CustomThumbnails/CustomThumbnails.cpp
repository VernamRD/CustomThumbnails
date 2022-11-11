//Copyright © 2022 TIGan. All Rights Reserved.

#include "CustomThumbnails.h"
#include "ContentBrowserModule.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "CustomThumbnailsStyle/CustomThumbnailsStyle.h"
#include "CustomThumbnailEditor/CustomThumbnailEditorSummoner.h"

IMPLEMENT_MODULE(FCustomThumbnailsModule, CustomThumbnails)

DEFINE_LOG_CATEGORY(LogCTModule);

#define LOCTEXT_NAMESPACE "FCustomThumbnailsModule"

const FName FCustomThumbnailsModule::ThumbnailPickerTabName = "CustomThumbnailEditor";

FCustomThumbnailsModule::FCustomThumbnailsModule()
{
    CustomThumbnailEditorSummoner = nullptr;
    MenuExtender = nullptr;
    MenuExtension = nullptr;
}

void FCustomThumbnailsModule::StartupModule()
{
    // Init style
    FCustomThumbnailsStyle::Initialize();
    FCustomThumbnailsStyle::ReloadTextures();

    // Create editor summoner
    CustomThumbnailEditorSummoner = MakeShareable(new FCustomThumbnailEditorSummoner);

    // Extend asset action menu
    MenuExtender = MakeShared<FExtender>();
    MenuExtension = MenuExtender->AddMenuExtension(
        "CreateBlueprintUsing",
        EExtensionHook::After,
        TSharedPtr<FUICommandList>(),
        FMenuExtensionDelegate::CreateRaw(this, &FCustomThumbnailsModule::AddMenuEntry)
        );

    // Add delegate in content browser context content menu action
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
    TArray<FContentBrowserMenuExtender_SelectedAssets>& MenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
    MenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FCustomThumbnailsModule::CustomExtender));
}

void FCustomThumbnailsModule::ShutdownModule()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
    TArray<FContentBrowserMenuExtender_SelectedAssets>& MenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();

    MenuExtender->RemoveExtension(MenuExtension.ToSharedRef());

    FCustomThumbnailsStyle::Shutdown();
}

TSharedRef<FExtender> FCustomThumbnailsModule::CustomExtender(const TArray<FAssetData>& Assets)
{
    SelectedAssets = Assets;

    return MenuExtender.ToSharedRef();
}

void FCustomThumbnailsModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
    MenuBuilder.AddMenuEntry(
        FText::FromString("Add Custom Thumbnail"),
        FText::FromString("Add a thumbnail for this asset and all its children"),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateRaw(this, &FCustomThumbnailsModule::OnAddCustomThumbnailButtonClicked))
        );
}

void FCustomThumbnailsModule::OnAddCustomThumbnailButtonClicked()
{
    check(CustomThumbnailEditorSummoner);

    CustomThumbnailEditorSummoner->CreateEditor(SelectedAssets);
    FGlobalTabmanager::Get()->TryInvokeTab(ThumbnailPickerTabName);
}

#undef LOCTEXT_NAMESPACE
