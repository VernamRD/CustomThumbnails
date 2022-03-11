//Copyright © 2022 TIGan. All Rights Reserved.

#include "CustomThumbnailPickerTab.h"

#include "PropertyCustomizationHelpers.h"
#include "CustomThumbnails/CustomThumbnails.h"
#include "CustomThumbnails/CustomRenderers/CustomCapturerThumbnail.h"
#include "Widgets/TextureAssetProperty.h"
#include "Widgets/CustomAssetProperty.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"

DEFINE_LOG_CATEGORY(LogCTThumbnailPickerTab);

#define LOCTEXT_NAMESPACE "FCustomThumbnailPickerTab"

FCustomThumbnailPickerTab::FCustomThumbnailPickerTab(const TArray<FAssetData>& Assets)
    : CTModule(FModuleManager::GetModuleChecked<FCustomThumbnailsModule>(TEXT("CustomThumbnails")))
{
    SelectedAssets = Assets;

    StartupTab();
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

    TSharedPtr<SBox> MainSizeBox;
    TSharedPtr<SVerticalBox> MainVerticalBox;

    TSharedRef<SDockTab> Tab = SNew(SDockTab)
                               .TabRole(ETabRole::NomadTab)
                               .ContentPadding(FMargin(10.0f, 5.0f, 10.0f, 10.0f))
                               [
                                   SAssignNew(MainSizeBox, SBox)
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
                                   ]
                               ];
    CreateEntryBoxList(SelectedAssets);

    OwnerTab = Tab;
    return Tab;
}

FReply FCustomThumbnailPickerTab::OnApply()
{
    UE_LOG(LogCTThumbnailPickerTab, VeryVerbose, TEXT("Starting to prepare an array of selected Assets"))

    // Prepare array asset for capture
    for (int32 Index = 0; Index < AssetProperties.Num(); Index++)
    {
        SelectedAssetsForThumbnail.Add(AssetProperties[Index].Get().GetSelectedAsset());
        
        //If the option is set to one thumbnail for all the assets, then only the zero index of the array of texture assets is used
        FAssetData TextureAsset = bOneThumbnailForAll
        ? TextureAssetProperties[0].Get().GetSelectedAsset()
        : TextureAssetProperties[Index].Get().GetSelectedAsset();
        SelectedTextureAssetsForThumbnail.Add(TextureAsset);
    }

    // Capture thumbnails
    CustomCapturerThumbnail::CaptureNewCustomThumbnails(SelectedAssetsForThumbnail, SelectedTextureAssetsForThumbnail);

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
            CreateEntryBoxList(SelectedAssets);
            break;
        }
        case ECheckBoxState::Unchecked:
        {
            bOneThumbnailForAll = false;
            CreateEntryBoxList(SelectedAssets);
            break;
        }
        case ECheckBoxState::Undetermined:
        {
            break;
        }
    }
}

void FCustomThumbnailPickerTab::CreateEntryBoxList(const TArray<FAssetData>& Assets)
{
    if (ensure(!Assets.IsEmpty()))
    {
        // Clear ScrollBox before fill
        AssetScrollBox->ClearChildren();

        for (int32 Index = 0; Index < SelectedAssets.Num(); ++Index)
        {
            // Check target asset
            if (Cast<UTexture2D>(SelectedAssets[Index].GetAsset()) != nullptr
                || Cast<UMaterial>(SelectedAssets[Index].GetAsset()))
                continue;

            TSharedPtr<SBox> AssetPropertySizeBox;
            TSharedPtr<SBox> TextureAssetPropertySizeBox;

            TSharedPtr<SBox> AssetRowBox = SNew(SBox)
                                           [
                                               SNew(SHorizontalBox)
                                               + SHorizontalBox::Slot()
                                               .Padding(FMargin(0.0f, 0.0f, 10.0f, 0.0f))
                                               [
                                                   SAssignNew(AssetPropertySizeBox, SBox) // Asset EntryBox
                                               ]
                                               + SHorizontalBox::Slot()
                                               [
                                                   SAssignNew(TextureAssetPropertySizeBox, SBox) // Texture asset EntryBox
                                               ]
                                           ];
            

            // Add entry boxes in local arrays and insert Initialized entry boxes and insert in Asset entry boxes selected assets
            
            AssetProperties.Add(MakeShareable(new FCustomAssetProperty(SelectedAssets[Index].GetAsset())));
            AssetPropertySizeBox->SetContent(AssetProperties[Index].Get().Initialize().ToSharedRef());
            if (!bOneThumbnailForAll || Index == 0)
            {
                TextureAssetProperties.Add(MakeShareable(new FTextureAssetProperty));
                TextureAssetPropertySizeBox->SetContent(TextureAssetProperties[Index].Get().Initialize().ToSharedRef());
            }

            // Add and attach slot in ScrollBox
            auto& ScrollBoxSlot = AssetScrollBox->AddSlot();
            ScrollBoxSlot.AttachWidget(AssetRowBox.ToSharedRef());
        }
    }
}

#undef LOCTEXT_NAMESPACE
