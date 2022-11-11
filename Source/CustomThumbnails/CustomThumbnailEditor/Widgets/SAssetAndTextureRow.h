// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SAssetPropertyEntryBox.h"

class SAssetAndTextureRow;

DECLARE_DELEGATE_OneParam(FOnRemoveRow, TSharedRef<SAssetAndTextureRow>);

/** A widget that creates a row from a pair of property boxes an asset for a new thumbnail and a texture asset */ 
class CUSTOMTHUMBNAILS_API SAssetAndTextureRow : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SAssetAndTextureRow) {}
    /** Called to check whether the asset should be filtered */
    SLATE_EVENT(FOnShouldFilterAsset, OnShouldFilterAsset)
    /** Called when the asset changes */
    SLATE_EVENT(FOnAssetChanged, OnAssetChanged)
    /** Called when the asset changes */
    SLATE_EVENT(FOnRemoveRow, OnRemoveRow)
    /** Asset that should be installed immediately after creating the widget */
    SLATE_ARGUMENT(FAssetData, SelectedAsset)
    /** Texture asset that should be installed immediately after creating the widget */
    SLATE_ARGUMENT(FAssetData, SelectedTextureAsset)
    /** Should be created texture asset property box? */
    SLATE_ATTRIBUTE(bool, OnShouldBeTextureAssetProperty)
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

    TPair<FAssetData, FAssetData> GetSelectedAssets() const;
protected:
    void OnButtonRemoveRowClicked();
    
private:
    FOnRemoveRow OnRemoveRowDelegate;
    
    // EntryBox widgetы for assets
    TSharedPtr<SAssetPropertyEntryBox> TextureAssetProperty;
    TSharedPtr<SAssetPropertyEntryBox> AssetProperty;

    TSharedPtr<SWidget> RowRemoveButton;
};
