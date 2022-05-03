//Copyright © 2022 TIGan. All Rights Reserved.

#include "CustomThumbnailPickerTab.h"

#include "PropertyCustomizationHelpers.h"
#include "CustomThumbnails/CustomThumbnails.h"
#include "CustomThumbnails/CustomRenderers/CustomCapturerThumbnail.h"

#include "Styling/ToolBarStyle.h"

#include "Widgets/SAssetPropertyEntryBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

DEFINE_LOG_CATEGORY(LogCTThumbnailPickerTab);

#define LOCTEXT_NAMESPACE "FCustomThumbnailPickerTab"

FCustomThumbnailPickerTab::FCustomThumbnailPickerTab(const TArray<FAssetData>& Assets)
    : CTModule(FModuleManager::GetModuleChecked<FCustomThumbnailsModule>(TEXT("CustomThumbnails")))
{
    SelectedAssets = Assets;

    StartupTab();
}

FCustomThumbnailPickerTab::~FCustomThumbnailPickerTab()
{
    // Clear memory
    SelectedAssets.Empty();
    SelectedTextureAssets.Empty();
    
    if(!OwnerTab.Get()->RequestCloseTab())
    {
        for(auto AssetProperty : AssetProperties)
        {
            AssetProperty.Reset();
        }

        for(auto TextureAssetProperty : TextureAssetProperties)
        {
            TextureAssetProperty.Reset();
        }

        AssetScrollBox.Reset();
        OwnerTab.Reset();
    };

    AssetProperties.Empty();
    TextureAssetProperties.Empty();
}


void FCustomThumbnailPickerTab::StartupTab()
{
    UE_LOG(LogCTThumbnailPickerTab, VeryVerbose, TEXT("StartupTab"));
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CTModule.GetThumbnailPickerTabName(),
                                FOnSpawnTab::CreateRaw(this, &FCustomThumbnailPickerTab::OnSpawnTab))
                            .SetDisplayName(FText::FromString("ThumbnailPicker"))
                            .SetIcon(FSlateIcon("CustomThumbnailsStyle", "ContentBrowser.AssetAction.AddThumbnail"))
                            .SetMenuType(ETabSpawnerMenuType::Hidden);
}

TSharedRef<SDockTab> FCustomThumbnailPickerTab::OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    UE_LOG(LogCTThumbnailPickerTab, VeryVerbose, TEXT("Start slate tab"));

    TSharedPtr<SVerticalBox> MainVerticalBox;

    TSharedRef<SDockTab> Tab = SNew(SDockTab)
                               .TabRole(ETabRole::NomadTab)
                               .ContentPadding(FMargin(10.0f, 5.0f, 10.0f, 10.0f))
                               [

                                   SAssignNew(MainVerticalBox, SVerticalBox)
                                   + SVerticalBox::Slot() // Header
                                     .AutoHeight()
                                     .Padding(0.0f, 5.0f, 0.0f, 5.0f)
                                   [
                                       SNew(SHorizontalBox)
                                       + SHorizontalBox::Slot()
                                         .FillWidth(1.0f)
                                         .Padding(0.0f, 0.0f, 5.0f, 0.0f)
                                         .VAlign(VAlign_Center)
                                       [
                                           SNew(STextBlock)
                                           .Text(LOCTEXT("ChooseObjectForThumbnail", "Select the object to assign the thumbnail"))
                                       ]
                                       + SHorizontalBox::Slot()
                                         .FillWidth(1.0f)
                                         .VAlign(VAlign_Center)
                                       [
                                           SNew(SHorizontalBox)
                                           + SHorizontalBox::Slot()
                                             .VAlign(VAlign_Center)
                                             .FillWidth(1.0f)
                                           [
                                               SNew(STextBlock)
                                               .Text(LOCTEXT("ChooseTextureForThumbnail",
                                                   "Select the desired texture for the thumbnail assignment"))
                                           ]
                                           + SHorizontalBox::Slot()
                                             .HAlign(HAlign_Right)
                                             .AutoWidth()
                                             .VAlign(VAlign_Center)
                                           [
                                               SNew(STextBlock)
                                               .Text(LOCTEXT("OneTextureForAll", "One for all?"))
                                           ]
                                           + SHorizontalBox::Slot() // CheckBox
                                             .HAlign(HAlign_Right)
                                             .Padding(5.0f)
                                             .AutoWidth()
                                             .VAlign(VAlign_Center)
                                           [
                                               SNew(SCheckBox)
                                                   .OnCheckStateChanged(this, &FCustomThumbnailPickerTab::OnCheckBoxChanged)
                                                   .IsChecked(ECheckBoxState::Unchecked)
                                           ]
                                       ]

                                   ]
                                   + SVerticalBox::Slot() // Assets entry boxes
                                     .VAlign(VAlign_Top)
                                     .FillHeight(true)
                                   [
                                       SAssignNew(AssetScrollBox, SScrollBox)
                                       .AnimateWheelScrolling(true)
                                   ]
                                   + SVerticalBox::Slot() // Buttons
                                     .VAlign(VAlign_Bottom)
                                     .HAlign(HAlign_Right)
                                     .AutoHeight()
                                   [
                                       SNew(SBox)
                                       .HeightOverride(50.0f)
                                       [
                                           SNew(SHorizontalBox)
                                           + SHorizontalBox::Slot()
                                             .Padding(12.0f)
                                             .HAlign(HAlign_Right)
                                             .AutoWidth()
                                           [
                                               SNew(SButton) // Apply
                                                   .OnClicked(this, &FCustomThumbnailPickerTab::OnApply)
                                                   .Text(LOCTEXT("ApplyButtonThumbnailPrickerTab", "Apply"))
                                           ]
                                           + SHorizontalBox::Slot()
                                             .Padding(12.0)
                                             .HAlign(HAlign_Right)
                                             .AutoWidth()

                                           [
                                               SNew(SButton) // Cancel
                                                   .OnClicked(this, &FCustomThumbnailPickerTab::OnCancel)
                                                   .Text(LOCTEXT("CancelButtonThumbnailPickerTab", "Cancel"))
                                           ]
                                       ]
                                   ]

                               ];
    //Tab->SetBorderImage(FStyleDefaults::);
    Tab->SetBorderBackgroundColor(FLinearColor::Green);

    CreateEntryBoxList();

    OwnerTab = Tab;
    return Tab;
}

void FCustomThumbnailPickerTab::CreateEntryBoxList()
{
    if (!SelectedAssets.IsEmpty())
    {
        // Reading the selected assets in the asset property before deleting them, since the set may differ from the original one
        ReadFromProperties();

        // Clear ScrollBox before fill
        AssetScrollBox->ClearChildren();
        AssetProperties.Empty();
        TextureAssetProperties.Empty();

        // Fill rows
        for (int32 Index = 0; Index < SelectedAssets.Num(); ++Index)
        {
            // Check target asset
            if (Cast<UTexture2D>(SelectedAssets[Index].GetAsset())
                || Cast<UMaterial>(SelectedAssets[Index].GetAsset()))
                continue;

            TSharedPtr<SHorizontalBox> AssetRow;
            TSharedPtr<SAssetPropertyEntryBox> AssetEntryBox;
            TSharedPtr<SAssetPropertyEntryBox> TextureAssetEntryBox;

            TSharedPtr<SBox> AssetRowBox = SNew(SBox)
                                           [
                                               SAssignNew(AssetRow, SHorizontalBox)
                                               + SHorizontalBox::Slot()
                                               .Padding(0.0f, 0.0f, 2.0f, 0.0f)
                                               [
                                                   SNew(SBox) // UObject asset EntryBox
                                                   [
                                                       SAssignNew(AssetEntryBox, SAssetPropertyEntryBox)
                                                       .AllowedClass(UObject::StaticClass())
                                                       .FilteredClasses(TSet<UClass*>{UTexture2D::StaticClass(), UMaterial::StaticClass()})
                                                       .SelectedObject(SelectedAssets[Index])
                                                       .OnAssetChanged(this, &FCustomThumbnailPickerTab::OnAssetChanged)
                                                       .OnShouldFilterAsset(this, &FCustomThumbnailPickerTab::OnShouldFilterAsset)
                                                   ]
                                               ]
                                           ];

            // Add texture asset entry box
            if (!bOneThumbnailForAll || Index == 0)
            {
                // Variable for storing the first valid SelectedTextureAsset element
                FAssetData FirstElement = FAssetData();

                /** Reading an array to get the first valid texture asset,
                 * so that when changing the bOneForAll mode to true,
                 * you do not have to re-select a single element */
                if(Index == 0)
                {
                    for (const auto SelectedTextureAsset : SelectedTextureAssets)
                    {
                        if(SelectedTextureAsset.IsValid())
                        {
                            FirstElement = SelectedTextureAsset;
                            break;
                        }
                    }
                }
                
                AssetRow->AddSlot()
                        .HAlign(HAlign_Center)
                        .AutoWidth()
                [
                    SNew(SSeparator)
                    .Orientation(Orient_Vertical)
                    .ColorAndOpacity(FLinearColor::Black)
                    .Thickness(2.0f)
                ];

                AssetRow->AddSlot()
                        .Padding(2.0f, 0.0f, 0.0f, 0.0f)
                [
                    SNew(SBox) // Texture asset EntryBox
                    [
                        SAssignNew(TextureAssetEntryBox, SAssetPropertyEntryBox)
                        .AllowedClass(UTexture2D::StaticClass())
                        .SelectedObject(Index == 0 ? FirstElement : FAssetData())
                    ]
                ];

                // Add asset entry boxes in local array
                TextureAssetProperties.Add(TextureAssetEntryBox);
            }

            // Add asset entry boxes in local array
            AssetProperties.Add(AssetEntryBox);

            AssetScrollBox->AddSlot().AttachWidget(AssetRowBox.ToSharedRef());
        }
    }
}

TArray<FAssetData>& FCustomThumbnailPickerTab::ReadFromProperties()
{
    // Read and fill selected texture assets
    if (!AssetProperties.IsEmpty())
    {
        // Clear array before fill
        SelectedAssets.Empty();

        // Read and fill selected assets
        for (const auto AssetProperty : AssetProperties)
        {
            // Validation check
            const FAssetData Asset = AssetProperty->GetSelectedAsset();
            const UObject* AssetObject = Cast<UObject>(Asset.GetAsset());

            if (AssetObject)
            {
                SelectedAssets.Add(Asset);
            }
        }
    }

    if (!TextureAssetProperties.IsEmpty())
    {
        SelectedTextureAssets.Empty();

        for (const auto TextureAssetProperty : TextureAssetProperties)
        {
            const FAssetData TextureAsset = TextureAssetProperty->GetSelectedAsset();
            const UTexture2D* TextureObject = Cast<UTexture2D>(TextureAsset.GetAsset());

            if (TextureObject)
            {
                SelectedTextureAssets.Add(TextureAsset);
            }
            else if(!bOneThumbnailForAll)
            {
                SelectedTextureAssets.Add(FAssetData());
            }
        }
    }

    return SelectedAssets;
}


FReply FCustomThumbnailPickerTab::OnApply()
{
    UE_LOG(LogCTThumbnailPickerTab, VeryVerbose, TEXT("Starting to prepare an array of selected Assets"))

    ReadFromProperties();
    
    // If the "One for all" option is selected, then we clear the array and put only one element.
    if(bOneThumbnailForAll)
    {
        const FAssetData SelectedTextureAssetTemp = SelectedTextureAssets[0];
        SelectedTextureAssets.Empty();
        SelectedTextureAssets.Add(SelectedTextureAssetTemp);
    }
    
    
    // Capture thumbnails
    FCustomCapturerThumbnail::CaptureNewCustomThumbnails(SelectedAssets, SelectedTextureAssets);

    // Close tab
    OwnerTab->RequestCloseTab();

    return FReply::Handled();
}

FReply FCustomThumbnailPickerTab::OnCancel()
{
    // Close tab
    OwnerTab->RequestCloseTab();

    return FReply::Handled();
}

void FCustomThumbnailPickerTab::OnCheckBoxChanged(ECheckBoxState CheckBoxState)
{
    switch (CheckBoxState)
    {
        case ECheckBoxState::Checked:
        {
            bOneThumbnailForAll = true;
            CreateEntryBoxList();
            break;
        }
        case ECheckBoxState::Unchecked:
        {
            bOneThumbnailForAll = false;
            CreateEntryBoxList();
            break;
        }
        case ECheckBoxState::Undetermined:
        {
            break;
        }
    }
}

bool FCustomThumbnailPickerTab::OnShouldFilterAsset(const FAssetData& AssetData) const
{
    return SelectedAssets.Contains(AssetData) ? true : false;
}

void FCustomThumbnailPickerTab::OnAssetChanged(const FAssetData& AssetData)
{
    ReadFromProperties();
}


#undef LOCTEXT_NAMESPACE
