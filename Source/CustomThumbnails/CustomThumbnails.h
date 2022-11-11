//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"

class FCustomThumbnailEditorSummoner;
class FWorkflowTabFactory;
class FSlateStyleSet;
class FCustomThumbnailEditor;
class FContentBrowserMenuExtension;

DECLARE_LOG_CATEGORY_EXTERN(LogCTModule, All, All);

class CUSTOMTHUMBNAILS_API FCustomThumbnailsModule : public IModuleInterface
{
public:
    FCustomThumbnailsModule();
    
#pragma region IModuleInterface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
#pragma endregion

    /** Name of tab for custom thumbnails editor */
    static const FName ThumbnailPickerTabName;

protected:
    /** Create extender and init extend asset menu in ContentBrowserMenuExtension     
     * @param Assets Selected assets
     * @return MenuExtender
    */
    TSharedRef<FExtender> CustomExtender(const TArray<FAssetData>& Assets);

    /** Callback of menu extender for add new menu action */
    void AddMenuEntry(FMenuBuilder& MenuBuilder);
    /** Callback of button in asset menu action */
    void OnAddCustomThumbnailButtonClicked();

    /** Pointer of custom thumbnails editor summoner */
    TSharedPtr<FCustomThumbnailEditorSummoner> CustomThumbnailEditorSummoner;
    
    /** Asset action menu extender */
    TSharedPtr<FExtender> MenuExtender;

    /** Asset action base extender. Need for delete delegate */
    TSharedPtr<const FExtensionBase> MenuExtension;

    /** Cached selected assets in content browser */
    TArray<FAssetData> SelectedAssets;
};