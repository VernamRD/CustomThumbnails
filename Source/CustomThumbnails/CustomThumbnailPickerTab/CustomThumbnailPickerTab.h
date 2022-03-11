//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassViewerModule.h"


class FCustomThumbnailsModule;
class FTextureAssetProperty;
class FCustomAssetProperty;
class FReply;
class SScrollBox;

DECLARE_LOG_CATEGORY_EXTERN(LogCTThumbnailPickerTab, All, All)

class CUSTOMTHUMBNAILS_API FCustomThumbnailPickerTab : public TSharedFromThis<FCustomThumbnailPickerTab>
{
public:
    explicit FCustomThumbnailPickerTab(const TArray<FAssetData>& Assets);

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

    // CheckBox callback
    void OnCheckBoxChanged(ECheckBoxState CheckBoxState);

protected:
    /** Filling AssetScrollBox with EntryBox lines of the pair Asset - Texture, with pre-cleaning.
     * @param Assets Assets for which an AssetEntryBox Asset - Texture pair must be created
     */
    void CreateEntryBoxList(const TArray<FAssetData>& Assets);

    // Selected assets from content browser
    TArray<FAssetData> SelectedAssets;

    // EntryBox widget arrays for assets
    TArray<TSharedRef<FTextureAssetProperty>> TextureAssetProperties;
    TArray<TSharedRef<FCustomAssetProperty>> AssetProperties;

    // Prepared asset arrays for installing custom miniatures
    TArray<FAssetData> SelectedAssetsForThumbnail;
    TArray<FAssetData> SelectedTextureAssetsForThumbnail;

    // Reference on MainModule
    FCustomThumbnailsModule& CTModule;

    // Pointers to some widgets
    TSharedPtr<SDockTab> OwnerTab;
    TSharedPtr<SScrollBox> AssetScrollBox;
    TSharedPtr<SBox> AssetEntryBoxesList;

    bool bOneThumbnailForAll = false;
};
