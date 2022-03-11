//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class SObjectPropertyEntryBox;

/*
 * A class for creating and storing the SObjectPropertyEntryBox widget
 * Needed to simplify the work with the widget,
 * because I was unable to adequately implement the necessary functionality in the Slate Framework
 */
class CUSTOMTHUMBNAILS_API FCustomAssetProperty : public TSharedFromThis<FCustomAssetProperty>
{
public:
    explicit FCustomAssetProperty(const UObject* AssetObject);

    /** Initializing a widget asset property
     *
     * @return Widget asset property
     */
    TSharedPtr<SObjectPropertyEntryBox> Initialize();

    /** Get selected asset in property box
     *
     *  @return Selected asset
     */
    FAssetData GetSelectedAsset() const { return SelectedAsset; }


protected:
    /** SObjectPropertyEntryBox pointer */
    TSharedPtr<SObjectPropertyEntryBox> PropertyEntryBox;

    /** Asset thumbnail pool */
    TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;

    /** Selected Asset */
    FAssetData SelectedAsset;

    // Callbacks
    void OnAssetChanged(const FAssetData& AssetData);
    FString OnGetSelectedPath() const;
};
