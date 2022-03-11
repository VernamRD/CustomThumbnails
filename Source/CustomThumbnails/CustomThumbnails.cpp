//Copyright © 2022 TIGan. All Rights Reserved.

#include "CustomThumbnails.h"

#include "ContentBrowserModule.h"

#include "AssetRegistryModule.h"
#include "ICustomThumbnailsModuleInterface.h"

#include "WatchPointViewer.h"
#include "Framework/Docking/LayoutExtender.h"

#include "CustomThumbnailsStyle/CustomThumbnailsStyle.h"

#include "CustomThumbnailPickerTab/CustomThumbnailPickerTab.h"

#include "LevelEditor.h"
#include "ToolMenu.h"
#include "ContentBrowserMenuExtension/ContentBrowserMenuExtension.h"
#include "Widgets/Docking/SDockTab.h"

DEFINE_LOG_CATEGORY(LogCTModule);

#define LOCTEXT_NAMESPACE "FCustomThumbnailsModule"

void FCustomThumbnailsModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    UE_LOG(LogCTModule, Verbose, TEXT("StartupModule"));

    FCustomThumbnailsStyle::Initialize();
    FCustomThumbnailsStyle::ReloadTextures();

    ICustomThumbnailsModuleInterface::StartupModule();

    auto& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
    auto& MenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
    MenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FCustomThumbnailsModule::CustomExtender));
    
    UE_LOG(LogCTModule, Display, TEXT("Custom Thumbnail Module is ready"));
}

void FCustomThumbnailsModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    ICustomThumbnailsModuleInterface::ShutdownModule();

    FCustomThumbnailsStyle::Shutdown();

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ThumbnailPickerTabName);
    
    UE_LOG(LogCTModule, Verbose, TEXT("ShutdownModule"));
}

void FCustomThumbnailsModule::CheckPointers()
{
    if (!PickerTabs.IsEmpty())
    {
        for (int32 Index = PickerTabs.Num() - 1; Index >= 0; --Index)
        {
            UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPicker %i "), Index);

            if (PickerTabs[Index].IsValid())
            {
                UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPicker %i is Valid"), Index);

                if (PickerTabs[Index]->GetTab().IsValid())
                {
                    UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPickerTab %i Is Valid"), Index);
                    if (PickerTabs[Index]->GetTab()->IsParentValid())
                    {
                        UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPickerTab %i Is Parent Valid"), Index);
                        continue;
                    }
                    else
                    {
                        UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPickerTab %i Is Parent Invalid"), Index);
                    }
                    PickerTabs[Index]->GetTab().Reset();
                    UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPickerTab Reset"));
                }
                else
                {
                    UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPickerTab %i Is Invalid"), Index);
                }
                PickerTabs[Index].Reset();
                UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPicker Reset"));
            }
            else
            {
                UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPickerTab %i is Invalid"), Index);
            }

            PickerTabs.RemoveSingle(PickerTabs[Index]);
            UE_LOG(LogCTModule, VeryVerbose, TEXT("ThumbnailPickerTab %i has been Removed"), Index);
        }
    }
}

TSharedRef<FExtender> FCustomThumbnailsModule::CustomExtender(const TArray<FAssetData>& Assets)
{
    UE_LOG(LogCTModule, VeryVerbose, TEXT("CustomExtender Start"));

    // Create extension for content browser
    Extension = MakeShareable(new FContentBrowserMenuExtension(Assets));
    CheckPointers();
    // Preparing PickerTab
    PickerTabs.Add(MakeShareable(new FCustomThumbnailPickerTab(Assets)));

    const auto MenuExtender = MakeShared<FExtender>();
    
    MenuExtender->AddMenuExtension(                                                                            //
        "CreateBlueprintUsing",                                                                                //
        EExtensionHook::After,                                                                                 //
        TSharedPtr<FUICommandList>(),                                                                          //
        FMenuExtensionDelegate::CreateSP(Extension.ToSharedRef(), &FContentBrowserMenuExtension::AddMenuEntry) //
        );

    UE_LOG(LogCTModule, VeryVerbose, TEXT("CustomExtender End"));

    return MenuExtender;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCustomThumbnailsModule, CustomThumbnails)
