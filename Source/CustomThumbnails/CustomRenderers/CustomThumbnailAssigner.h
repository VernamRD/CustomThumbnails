//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/** Auxiliary class for installing custom thumbnails */
class FCustomThumbnailAssigner
{
public:
    /** Copying thumbnails from the Texture Assets array and applying them to AssetForNewThumbnail
     * @param AssetsForNewThumbnail Assets for which you need to change the thumbnail
     * @param TextureAssets Texture assets from which thumbnails will be taken.
     * 
     * If the array of TextureAssets does not match the size of the array of assets,
     * then only the zero index element will be used
     *
     * @return True if the thumbnails were successfully captured
     */
    static bool AssignNewThumbnails(TArray<FAssetData>& AssetsForNewThumbnail, TArray<FAssetData> TextureAssets);

protected:
    static bool AssignNewThumbnailInternal(const FAssetData& AssetForNewThumbnail, const FAssetData& TextureAsset);
};
