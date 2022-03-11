//Copyright © 2022 TIGan. All Rights Reserved.


#include "CustomCapturerThumbnail.h"

#include "Misc/FileHelper.h"
#include "UObject/ConstructorHelpers.h"
#include "ISourceControlModule.h"
#include "Engine/Texture2DArray.h"
#include "AssetViewUtils.h"
#include "ObjectTools.h"

DEFINE_LOG_CATEGORY(LogCTCustomRenderer);

bool CustomCapturerThumbnail::CaptureNewCustomThumbnails(TArray<FAssetData>& Assets, TArray<FAssetData> TextureAssets)
{
    UE_LOG(LogCTCustomRenderer, VeryVerbose, TEXT("Start register custom renderers"));

    if (Assets.IsEmpty() || TextureAssets.IsEmpty())
    {
        UE_LOG(LogCTCustomRenderer, Warning, TEXT("AssetsForRenderer is empty"));
        return false;
    }

    for (int32 Index = 0; Index < Assets.Num(); Index++)
    {
        FAssetData& Asset = Assets[Index];
        FAssetData& TextureAsset = TextureAssets.Num() == 1 ? TextureAssets[0] : TextureAssets[Index];

        UTexture2D* TextureObject = Cast<UTexture2D>(TextureAsset.GetAsset());
        if (TextureObject == nullptr) continue;
        
        // Setup object names
        const FString ObjectFullName = Asset.GetFullName();
        const FString PackageName = Asset.PackageName.ToString();

        // Setup texture names
        const FString TextureObjectFullName = TextureAsset.GetFullName();
        const FString TexturePackageName = TextureAsset.PackageName.ToString();

        // Get package assets
        UPackage* AssetPackage = FindObject<UPackage>(NULL, *PackageName);
        UPackage* TexturePackage = FindObject<UPackage>(NULL, *TexturePackageName);
        
        if (ensure(AssetPackage != nullptr && TexturePackage != nullptr))
        {
            // Get texture thumbnail
            if (ensure(TexturePackage->HasThumbnailMap()))
            {
                // Get thumbnail map
                const FThumbnailMap& TexturePackageThumbnailMap = TexturePackage->GetThumbnailMap();
                // Find in thumbnail map required thumbnail
                const FObjectThumbnail* TextureObjectThumbnail = ThumbnailTools::GetThumbnailForObject(TextureObject);

                if (ensure(TextureObjectThumbnail != nullptr))
                {
                    // Create temp thumbnail
                    FObjectThumbnail TempThumbnail = FObjectThumbnail(*TextureObjectThumbnail);

                    FObjectThumbnail* NewThumbnail = ThumbnailTools::CacheThumbnail(ObjectFullName, &TempThumbnail, AssetPackage);
                    if (ensure(NewThumbnail != nullptr))
                    {
                        // We need to indicate that the package needs to be resaved
                        AssetPackage->MarkPackageDirty();

                        // Let the content browser know that we've changed the thumbnail
                        NewThumbnail->MarkAsDirty();

                        // Signal that the asset was changed if it is loaded so thumbnail pools will update
                        if (Asset.IsAssetLoaded())
                        {
                            Asset.GetAsset()->PostEditChange();
                        }
                        NewThumbnail->SetCreatedAfterCustomThumbsEnabled();

                        UE_LOG(LogCTCustomRenderer, VeryVerbose, TEXT("The successful assignment of a new thumbnail for %s from %s"),
                            *Asset.AssetName.ToString(),
                            *TextureAsset.AssetName.ToString());
                    }
                }
            }
        }
    }
    return true;
}
