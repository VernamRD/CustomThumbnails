// Fill out your copyright notice in the Description page of Project Settings.


#include "SAssetAndTextureRow.h"

#include "SlateOptMacros.h"
#include "CustomThumbnails/CustomThumbnailsStyle/CustomThumbnailsStyle.h"
#include "Widgets/Layout/SSeparator.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "AssetAndTextureRow"

void SAssetAndTextureRow::Construct(const FArguments& InArgs)
{
    OnRemoveRowDelegate = InArgs._OnRemoveRow;

    RowRemoveButton = PropertyCustomizationHelpers::MakeDeleteButton(
        FSimpleDelegate::CreateRaw(this, &SAssetAndTextureRow::OnButtonRemoveRowClicked),
        LOCTEXT("RemoveRowButton", "Remove Row"));

    if (InArgs._OnShouldBeTextureAssetProperty.Get())
    {
        ChildSlot
        [
            SNew(SBorder)
            .BorderImage(FCustomThumbnailsStyle::Get().GetBrush("CustomThumbnailEditor.Row.Background"))
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                  .Padding(0.0f, 0.0f, 2.0f, 0.0f)
                  .FillWidth(0.5f)
                [
                    // ASSET ENTRY BOX
                    SNew(SBox)
                    .HAlign(HAlign_Left)
                    [
                        SAssignNew(AssetProperty, SAssetPropertyEntryBox)
                        .AllowedClass(UObject::StaticClass())
                        .FilteredClasses(TSet<UClass*>{UTexture2D::StaticClass(), UMaterial::StaticClass()})
                        .SelectedObject(InArgs._SelectedAsset)
                        .OnAssetChanged(InArgs._OnAssetChanged)
                        .OnShouldFilterAsset(InArgs._OnShouldFilterAsset)
                    ]
                ]
                + SHorizontalBox::Slot()
                  .HAlign(HAlign_Right)
                  .AutoWidth()
                [
                    // SEPARATOR
                    SNew(SSeparator)
                    .Orientation(Orient_Vertical)
                    .ColorAndOpacity(FLinearColor::Gray)
                    .Thickness(1.0f)
                ]
                + SHorizontalBox::Slot()
                  .Padding(2.0f, 0.0f)
                  .FillWidth(0.5f)
                [
                    SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    [
                        SNew(SBox)
                        .HAlign(HAlign_Left)
                        [
                            SAssignNew(TextureAssetProperty, SAssetPropertyEntryBox)
                        .AllowedClass(UTexture2D::StaticClass())
                        .OnAssetChanged(InArgs._OnAssetChanged)
                        .SelectedObject(InArgs._SelectedTextureAsset)
                        ]
                    ]
                    + SHorizontalBox::Slot()
                      .AutoWidth()
                      .HAlign(HAlign_Right)
                      .VAlign(VAlign_Center)
                    [
                        RowRemoveButton.ToSharedRef()
                    ]

                ]
            ]
        ];
    }
    else
    {
        ChildSlot
        [
            SNew(SBorder)
            .BorderImage(FCustomThumbnailsStyle::Get().GetBrush("CustomThumbnailEditor.Row.Background"))
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                  .Padding(0.0f, 0.0f, 2.0f, 0.0f)
                  .FillWidth(1.0f)
                [
                    // ASSET ENTRY BOX
                    SNew(SBox)
                    .HAlign(HAlign_Left)
                    [
                        SAssignNew(AssetProperty, SAssetPropertyEntryBox)
                        .AllowedClass(UObject::StaticClass())
                        .FilteredClasses(TSet<UClass*>{UTexture2D::StaticClass(), UMaterial::StaticClass()})
                        .SelectedObject(InArgs._SelectedAsset)
                        .OnAssetChanged(InArgs._OnAssetChanged)
                        .OnShouldFilterAsset(InArgs._OnShouldFilterAsset)
                    ]
                ]
                + SHorizontalBox::Slot()
                  .AutoWidth()
                  .HAlign(HAlign_Right)
                  .VAlign(VAlign_Center)
                  .Padding(2.0f, 0.0f)
                [
                    RowRemoveButton.ToSharedRef()
                ]
            ]
        ];
    }
}

TPair<FAssetData, FAssetData> SAssetAndTextureRow::GetSelectedAssets() const
{
    return TPair<FAssetData, FAssetData>(AssetProperty->GetSelectedAsset(),
        TextureAssetProperty ? TextureAssetProperty->GetSelectedAsset() : FAssetData());
}

void SAssetAndTextureRow::OnButtonRemoveRowClicked()
{
    OnRemoveRowDelegate.ExecuteIfBound(SharedThis(this));
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
