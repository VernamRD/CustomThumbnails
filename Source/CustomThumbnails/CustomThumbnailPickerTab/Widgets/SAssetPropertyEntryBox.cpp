//Copyright © 2022 TIGan. All Rights Reserved.


#include "SAssetPropertyEntryBox.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SAssetPropertyEntryBox::Construct(const FArguments& InArgs)
{
    this->SelectedAsset = InArgs._SelectedObject;
    this->FilteredClasses = InArgs._FilteredClasses;
    this->OnShouldFilterAssetDelegate = InArgs._OnShouldFilterAsset;
    this->OnAssetChangedDelegate = InArgs._OnAssetChanged;

    AssetThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));

    SObjectPropertyEntryBox::Construct(SObjectPropertyEntryBox::FArguments()
                                       .AllowedClass(InArgs._AllowedClass)
                                       .DisplayThumbnail(true)
                                       .AllowClear(true)
                                       .ThumbnailPool(AssetThumbnailPool)
                                       .OnObjectChanged(this, &SAssetPropertyEntryBox::OnAssetChanged)
                                       .OnShouldFilterAsset(this, &SAssetPropertyEntryBox::OnShouldFilterAsset)
                                       .ObjectPath(this, &SAssetPropertyEntryBox::OnGetSelectedPath));
}

void SAssetPropertyEntryBox::OnAssetChanged(const FAssetData& AssetData)
{
    UObject* SelectedObject = Cast<UObject>(AssetData.GetAsset());

    if (SelectedObject)
    {
        SelectedAsset = SelectedObject;
        OnAssetChangedDelegate.ExecuteIfBound(AssetData);
    }
    else
    {
        SelectedAsset = nullptr;
    }
}

FString SAssetPropertyEntryBox::OnGetSelectedPath() const
{
    return SelectedAsset.IsValid() ? SelectedAsset.ObjectPath.ToString() : FString(TEXT("None"));
}

bool SAssetPropertyEntryBox::OnShouldFilterAsset(const FAssetData& AssetData) const
{
    if (!FilteredClasses.IsEmpty() && FilteredClasses.Contains(AssetData.GetAsset()->GetClass())) return true;

    return OnShouldFilterAssetDelegate.IsBound() ? OnShouldFilterAssetDelegate.Execute(AssetData) : false;
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
