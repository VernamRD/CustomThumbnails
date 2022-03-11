//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class IAssetRegistry;
class FCustomThumbnailsModule;

DECLARE_LOG_CATEGORY_EXTERN(LogCTContentBrowserMenuExtension, All, All);

class CUSTOMTHUMBNAILS_API FContentBrowserMenuExtension : public TSharedFromThis<FContentBrowserMenuExtension>
{
public:
    explicit FContentBrowserMenuExtension(const TArray<FAssetData>& Assets);
   
    void AddMenuEntry(FMenuBuilder& MenuBuilder);
    void OnAddThumbnailButtonClicked();

    IAssetRegistry& AssetRegistry;
    FCustomThumbnailsModule& CTModule;

    const TArray<FAssetData>& SelectedAssets;
    
    TArray<FString> Paths;
};
