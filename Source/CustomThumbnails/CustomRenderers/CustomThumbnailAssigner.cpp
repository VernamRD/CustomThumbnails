//Copyright © 2022 TIGan. All Rights Reserved.

#include "CustomThumbnailAssigner.h"

#include "CustomThumbnails/CustomThumbnails.h"
#include "Engine/Texture2DArray.h"
#include "ObjectTools.h"

bool FCustomThumbnailAssigner::AssignNewThumbnails(TArray<FAssetData>& AssetsForNewThumbnail, TArray<FAssetData> TextureAssets)
{
    if (AssetsForNewThumbnail.IsEmpty() || TextureAssets.IsEmpty())
    {
        UE_LOG(LogCTModule, Warning, TEXT("AssetsForRenderer is empty"));
        return false;
    }

    for (int32 Index = 0; Index < AssetsForNewThumbnail.Num(); ++Index)
    {
        FAssetData& Asset = AssetsForNewThumbnail[Index];
        FAssetData& TextureAsset = TextureAssets.Num() == 1 || TextureAssets.Num() != AssetsForNewThumbnail.Num()
                                       ? TextureAssets[0]
                                       : TextureAssets[Index];

        AssignNewThumbnailInternal(Asset, TextureAsset);
    }
    return true;
}

bool FCustomThumbnailAssigner::AssignNewThumbnailInternal(const FAssetData& AssetForNewThumbnail, const FAssetData& TextureAsset)
{
    if(!AssetForNewThumbnail.GetAsset() || !TextureAsset.GetAsset()) return false;

    // Validate texture asset
    UTexture2D* TextureObject = Cast<UTexture2D>(TextureAsset.GetAsset());
    if (!TextureObject) return false;

    // Setup object names
    const FString ObjectFullName = AssetForNewThumbnail.GetFullName();
    const FString PackageName = AssetForNewThumbnail.PackageName.ToString();

    // Setup texture names
    const FString TexturePackageName = TextureAsset.PackageName.ToString();

    // Get package assets
    UPackage* AssetPackage = FindObject<UPackage>(nullptr, *PackageName);
    const UPackage* TexturePackage = FindObject<UPackage>(nullptr, *TexturePackageName);

    if (AssetPackage && TexturePackage)
    {
        const FObjectThumbnail* TextureObjectThumbnail;
        
        if(!TexturePackage->HasThumbnailMap())
        {
            // Generate thumbnail
            TextureObjectThumbnail = ThumbnailTools::GenerateThumbnailForObjectToSaveToDisk(TextureObject);
        }
        else
        {
            // Find in thumbnail map required thumbnail
            TextureObjectThumbnail = ThumbnailTools::GetThumbnailForObject(TextureObject);
        }
        
        // Get texture thumbnail
        if (ensureAlwaysMsgf(TextureObjectThumbnail, TEXT("Texture package does not have a thumbnail")))
        {
            if (ensureAlwaysMsgf(TextureObjectThumbnail != nullptr, TEXT("Texture object thumbnail is NULL")))
            {
                // Create temp thumbnail
                FObjectThumbnail TempThumbnail = FObjectThumbnail(*TextureObjectThumbnail);

                // Cache new thumbnail
                FObjectThumbnail* NewThumbnail = ThumbnailTools::CacheThumbnail(ObjectFullName, &TempThumbnail, AssetPackage);
                if (ensureAlwaysMsgf(NewThumbnail != nullptr, TEXT("New thumbnail is NULL")))
                {
                    // We need to indicate that the package needs to be resaved
                    AssetPackage->MarkPackageDirty();

                    // Let the content browser know that we've changed the thumbnail
                    NewThumbnail->MarkAsDirty();

                    // Signal that the asset was changed if it is loaded so thumbnail pools will update
                    if (AssetForNewThumbnail.IsAssetLoaded())
                    {
                        AssetForNewThumbnail.GetAsset()->PostEditChange();
                    }
                    NewThumbnail->SetCreatedAfterCustomThumbsEnabled();

                    UE_LOG(LogCTModule, VeryVerbose, TEXT("The successful assignment of a new thumbnail for %s from %s"),
                        *AssetForNewThumbnail.AssetName.ToString(),
                        *TextureAsset.AssetName.ToString());

                    return true;
                }
            }
        }
    }

    return false;
}
