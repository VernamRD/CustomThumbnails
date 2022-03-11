//Copyright © 2022 TIGan. All Rights Reserved.


#include "CustomAssetProperty.h"

#include "PropertyCustomizationHelpers.h"


FCustomAssetProperty::FCustomAssetProperty(const UObject* AssetObject)
{
    AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));

    SelectedAsset = AssetObject;
}

TSharedPtr<SObjectPropertyEntryBox> FCustomAssetProperty::Initialize()
{
    PropertyEntryBox = SAssignNew(PropertyEntryBox, SObjectPropertyEntryBox)
       .AllowedClass(UObject::StaticClass())
       .DisplayThumbnail(true)
       .ThumbnailPool(AssetThumbnailPool)
       .OnObjectChanged(this, &FCustomAssetProperty::OnAssetChanged)
       .ObjectPath(this, &FCustomAssetProperty::OnGetSelectedPath);

    return PropertyEntryBox;
}

void FCustomAssetProperty::OnAssetChanged(const FAssetData& AssetData)
{
    UObject* SelectedUObject = Cast<UObject>(AssetData.GetAsset());

    if (SelectedUObject)
    {
        SelectedAsset = SelectedUObject;
    }
    else
    {
        SelectedAsset = nullptr;
    }
}

FString FCustomAssetProperty::OnGetSelectedPath() const
{
    return SelectedAsset.IsValid() ? SelectedAsset.ObjectPath.ToString() : FString(TEXT("None"));
}
