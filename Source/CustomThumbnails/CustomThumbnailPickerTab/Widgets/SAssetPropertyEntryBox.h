//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PropertyCustomizationHelpers.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, FOnShouldFilterAsset, const FAssetData&)
DECLARE_DELEGATE_OneParam(FOnAssetChanged, const FAssetData&)

class CUSTOMTHUMBNAILS_API SAssetPropertyEntryBox : public SObjectPropertyEntryBox
{
public:
    SLATE_BEGIN_ARGS(SAssetPropertyEntryBox)
            : _AllowedClass(UObject::StaticClass()) {}

        /** Called to check whether the asset should be filtered */
        SLATE_EVENT(FOnShouldFilterAsset, OnShouldFilterAsset)
        /** Called when the asset changes */
        SLATE_EVENT(FOnAssetChanged, OnAssetChanged)
        /** The class that is being filtered by */
        SLATE_ARGUMENT(UClass*, AllowedClass)
        /** Asset that should be installed immediately after creating the widget */
        SLATE_ARGUMENT(FAssetData, SelectedObject)
        /** Array of classes to be filtered */
        SLATE_ARGUMENT(TSet<UClass*>, FilteredClasses)
    SLATE_END_ARGS()

    /** Constructs this widget with InArgs */
    void Construct(const FArguments& InArgs);

    /** @return Selected asset in the asset property */
    FAssetData GetSelectedAsset() const { return SelectedAsset; }

protected:
    /** Asset thumbnail pool */
    TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;
    
    // Callbacks.
    void OnAssetChanged(const FAssetData& AssetData);
    bool OnShouldFilterAsset(const FAssetData& AssetData) const;
    FString OnGetSelectedPath() const;

    /** Selected asset in the asset property */
    FAssetData SelectedAsset;

    /** Array of classes to be filtered */
    TSet<UClass*> FilteredClasses;

private:
    /** Delegate for determining whether an asset should be filtered, even if it is an allowed class */
    FOnShouldFilterAsset OnShouldFilterAssetDelegate;
    /** Delegate to call when the asset changes */
    FOnAssetChanged OnAssetChangedDelegate;
};
