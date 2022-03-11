//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class SObjectPropertyEntryBox;

/*
 * A class for creating and storing the SObjectPropertyEntryBox widget
 * Needed to simplify the work with the widget,
 * because I was unable to adequately implement the necessary functionality in the Slate Framework
 */
class CUSTOMTHUMBNAILS_API FTextureAssetProperty : public TSharedFromThis<FTextureAssetProperty>
{
public:
    FTextureAssetProperty();

    /** Initializing a widget asset property
     *
     * @return Widget asset property
     */
    TSharedPtr<SObjectPropertyEntryBox> Initialize();

    /** Get selected texture asset in property box
     *
     *  @return Selected texture asset
     */
    FAssetData GetSelectedAsset() const { return SelectedTextureAsset; }
    
protected:
    /** Asset widget */
    TSharedPtr<SObjectPropertyEntryBox> PropertyEntryBox;
    
    /** Asset thumbnail pool */
    TSharedPtr<FAssetThumbnailPool> AssetThumbnailPool;

    /** Selected texture asset */
    FAssetData SelectedTextureAsset;
    
    // Callbacks
    void OnTextureChanged(const FAssetData& AssetData);
    FString OnGetSelectedPath() const;
    
};
