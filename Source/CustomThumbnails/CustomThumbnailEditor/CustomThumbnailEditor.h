//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClassViewerModule.h"
#include "Widgets/SAssetPropertyEntryBox.h"


class SAssetAndTextureRow;
class FCustomThumbnailEditorSummoner;
class FCustomThumbnailsModule;
class FReply;
class SScrollBox;

class CUSTOMTHUMBNAILS_API FCustomThumbnailEditor : public TSharedFromThis<FCustomThumbnailEditor>
{
public:
    explicit FCustomThumbnailEditor(TWeakPtr<FCustomThumbnailEditorSummoner> Summoner, const TArray<FAssetData>& Assets);
    virtual ~FCustomThumbnailEditor();

    // Spawn tab callback
    TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs);
    
protected:
    void StartupEditor();
    
    /** Filling AssetScrollBox with rows of the pair Asset - Texture, with pre-cleaning. */
    void CreateRowList(const bool bReadFromRows = true);
    
    /** Add asset row in AssetScrollBox.
     * @note Should NOT be called somewhere other than CreateRowList!
     */
    void AddAssetRow(const FAssetData& Asset, const FAssetData& TextureAsset);
    
    /** Add asset row in AssetScrollBox. 
     * @note Can be called somewhere other than CreateRowList
     */
    void AddAssetRowInExistList(const FAssetData& Asset, const FAssetData& TextureAsset);
    
    /** Reading assets from the rows assets, if any, then adding them to the cleared array of selected assets
     * @param bReadEmptyAssets If true, empty assets from the strings will be read
     * @return SelectedAssets
     */
    TArray<FAssetData>& ReadFromRows(const bool bReadEmptyAssets = true);

    // Buttons callback
    FReply OnApply();
    FReply OnCancel();
    FReply OnAddNewAssetRowButtonClicked();
    void OnRemoveRowButtonClicked(TSharedRef<SAssetAndTextureRow> RemovedRow);
    
    // Callbacks
    void OnOneForAllCheckBoxChanged(ECheckBoxState CheckBoxState);
    bool OnShouldFilterAsset(const FAssetData& AssetData) const;
    void OnAssetChanged(const FAssetData& AssetData);
    FText GetTooltipOneForAll() const;
    
    /** Callback of tab closed */
    void OnCloseTab(TSharedRef<SDockTab> ClosedTab);

    /** Owner summoner ptr */
    TWeakPtr<FCustomThumbnailEditorSummoner> EditorSummonerPtr;
    
    /** An array of selected assets for applying a custom thumbnail.
     * By default, it is set based on the selected assets in the content browser */
    TArray<FAssetData> SelectedAssetsForThumbnail;

    /** An array of selected texture assets to get a custom thumbnail */
    TArray<FAssetData> SelectedTextureAssets;

    /** Cached selected texture assets, when select one thumbnail for all */
    TArray<FAssetData> CachedSelectedTextureAssets;

    // Row widgets array for assets
    TArray<TSharedPtr<SAssetAndTextureRow>> AssetAndTextureRows;

    // Pointers to some widgets
    TSharedPtr<SDockTab> Tab;
    TSharedPtr<SScrollBox> AssetScrollBox;

    /** Stores the status of the checkbox "One for all?" */
    bool bOneThumbnailForAll = false;
};
