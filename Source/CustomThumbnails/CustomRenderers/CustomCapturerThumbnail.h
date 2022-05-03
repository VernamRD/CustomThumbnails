//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCTCustomRenderer, All, All)

class FCustomCapturerThumbnail
{
public:
    /** Copying thumbnails from the Texture Assets array and applying them to Assets
     * @param Assets Assets for which you need to change the thumbnail
     * 
     * @param TextureAssets Texture assets from which thumbnails will be taken.
     * If the array does not match the size of the array of assets,
     * then only the zero index element will be used
     *
     * @return True if the thumbnails were successfully captured
     */
    static bool CaptureNewCustomThumbnails(TArray<FAssetData>& Assets, TArray<FAssetData> TextureAssets);
};
