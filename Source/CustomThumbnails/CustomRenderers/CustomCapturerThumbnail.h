//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCTCustomRenderer, All, All)

class CustomCapturerThumbnail
{
public:
    static bool CaptureNewCustomThumbnails(TArray<FAssetData>& Assets, TArray<FAssetData> TextureAssets);
};
