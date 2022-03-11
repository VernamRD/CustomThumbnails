//Copyright © 2022 TIGan. All Rights Reserved.


#include "TextureAssetProperty.h"
#include "PropertyCustomizationHelpers.h"

FTextureAssetProperty::FTextureAssetProperty()
{
    AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
}

TSharedPtr<SObjectPropertyEntryBox> FTextureAssetProperty::Initialize()
{
    
    PropertyEntryBox = SAssignNew(PropertyEntryBox, SObjectPropertyEntryBox)
       .AllowedClass(UTexture2D::StaticClass())
       .DisplayThumbnail(true)
       .ThumbnailPool(AssetThumbnailPool)
       .OnObjectChanged(this, &FTextureAssetProperty::OnTextureChanged)
       .ObjectPath(this, &FTextureAssetProperty::OnGetSelectedPath);

    return PropertyEntryBox;
    
}


void FTextureAssetProperty::OnTextureChanged(const FAssetData& AssetData)
{
    UTexture2D* ThumbnailTexture = Cast<UTexture2D>(AssetData.GetAsset());

    if (ThumbnailTexture)
    {
        SelectedTextureAsset = ThumbnailTexture;
    }
    else
    {
        SelectedTextureAsset = nullptr;
    }
}

FString FTextureAssetProperty::OnGetSelectedPath() const
{
    return SelectedTextureAsset.IsValid() ? SelectedTextureAsset.ObjectPath.ToString() : FString(TEXT("None"));
}
