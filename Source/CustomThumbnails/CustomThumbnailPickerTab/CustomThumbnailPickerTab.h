//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassViewerModule.h"
#include "Widgets/SAssetPropertyEntryBox.h"


class SObjectPropertyEntryBox;
class FCustomThumbnailsModule;
class FReply;
class SScrollBox;

DECLARE_LOG_CATEGORY_EXTERN(LogCTThumbnailPickerTab, All, All)

class CUSTOMTHUMBNAILS_API FCustomThumbnailPickerTab : public TSharedFromThis<FCustomThumbnailPickerTab>
{
public:
    explicit FCustomThumbnailPickerTab(const TArray<FAssetData>& Assets);
    virtual ~FCustomThumbnailPickerTab();

    void StartupTab();

    // Spawn tab callback
    TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs);

    /** Getter tab pointer
     * @return Tab pointer
     */
    TSharedPtr<SDockTab> GetTab() const { return OwnerTab; }

    // Buttons callback
    FReply OnApply();
    FReply OnCancel();
    
protected:
    /** Filling AssetScrollBox with EntryBox lines of the pair Asset - Texture, with pre-cleaning. */
    void CreateEntryBoxList();

    // Callbacks
    void OnCheckBoxChanged(ECheckBoxState CheckBoxState);
    bool OnShouldFilterAsset(const FAssetData& AssetData) const;
    void OnAssetChanged(const FAssetData& AssetData);
    
    /** An array of selected assets for applying a custom thumbnail.
     * By default, it is set based on the selected assets in the content browser */
    TArray<FAssetData> SelectedAssets;

    /** An array of selected texture assets to get a custom thumbnail */
    TArray<FAssetData> SelectedTextureAssets;

    // EntryBox widget arrays for assets
    TArray<TSharedPtr<SAssetPropertyEntryBox>> TextureAssetProperties;
    TArray<TSharedPtr<SAssetPropertyEntryBox>> AssetProperties;

    // Reference on MainModule
    FCustomThumbnailsModule& CTModule;

    // Pointers to some widgets
    TSharedPtr<SDockTab> OwnerTab;
    TSharedPtr<SScrollBox> AssetScrollBox;

    /** Stores the status of the checkbox "One for all?" */
    bool bOneThumbnailForAll = false;

private:
    /** Reading assets from the property assets, if any, then adding them to the cleared array of selected assets
     * @return SelectedAssets
     */
    TArray<FAssetData>& ReadFromProperties();
};
