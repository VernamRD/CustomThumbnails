//Copyright © 2022 TIGan. All Rights Reserved.

#include "CustomThumbnailEditor.h"

#include "CustomThumbnailEditorSummoner.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "CustomThumbnails/CustomThumbnails.h"
#include "CustomThumbnails/CustomRenderers/CustomThumbnailAssigner.h"
#include "CustomThumbnails/CustomThumbnailsStyle/CustomThumbnailsStyle.h"

#include "Styling/ToolBarStyle.h"
#include "Widgets/SAssetAndTextureRow.h"

#include "Widgets/SAssetPropertyEntryBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"

#define LOCTEXT_NAMESPACE "FCustomThumbnailEditor"

FCustomThumbnailEditor::FCustomThumbnailEditor(TWeakPtr<FCustomThumbnailEditorSummoner> Summoner, const TArray<FAssetData>& Assets)
{
    check(Summoner.IsValid());

    SelectedAssetsForThumbnail = Assets;
    EditorSummonerPtr = Summoner;

    StartupEditor();
}

FCustomThumbnailEditor::~FCustomThumbnailEditor()
{
    // Clear memory
    SelectedAssetsForThumbnail.Empty();
    SelectedTextureAssets.Empty();
}

void FCustomThumbnailEditor::StartupEditor()
{
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FCustomThumbnailsModule::ThumbnailPickerTabName,
                                FOnSpawnTab::CreateRaw(this, &FCustomThumbnailEditor::OnSpawnTab))
                            .SetDisplayName(FText::FromString(FCustomThumbnailsModule::ThumbnailPickerTabName.ToString()))
                            .SetIcon(FSlateIcon("CustomThumbnailsStyle", "ContentBrowser.AssetAction.AddThumbnail"))
                            .SetMenuType(ETabSpawnerMenuType::Hidden);
}

TSharedRef<SDockTab> FCustomThumbnailEditor::OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TSharedPtr<SVerticalBox> MainVerticalBox;

    Tab = SNew(SDockTab)
    .TabRole(ETabRole::NomadTab)
    .OnTabClosed(this, &FCustomThumbnailEditor::OnCloseTab)
          [
              SNew(SBorder)
                  .BorderImage(FCustomThumbnailsStyle::Get().GetBrush("CustomThumbnailEditor.Background"))
                  .Padding(FMargin(10.0f, 5.0f, 10.0f, 10.0f))
              [
                  SAssignNew(MainVerticalBox, SVerticalBox)

                  // HEADER
                  + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(0.0f, 5.0f, 0.0f, 5.0f)
                  [
                      SNew(SBorder)
                      .BorderImage(FCustomThumbnailsStyle::Get().GetBrush("CustomThumbnailEditor.Row.Background"))
                      [
                          SNew(SHorizontalBox)
                          + SHorizontalBox::Slot()
                            .FillWidth(0.5f)
                            .Padding(5.0f)
                            .VAlign(VAlign_Center)
                          [
                              SNew(STextBlock)
                              .Text(LOCTEXT("ChooseObjectForThumbnail", "Select the object to assign the thumbnail"))
                          ]
                          + SHorizontalBox::Slot()
                            .HAlign(HAlign_Left)
                            .AutoWidth()
                          [
                              SNew(SSeparator)
                                  .Orientation(Orient_Vertical)
                                  .ColorAndOpacity(FLinearColor::Gray)
                                  .Thickness(1.0f)
                          ]
                          + SHorizontalBox::Slot()
                            .FillWidth(0.5f)
                            .Padding(5.0f)
                            .VAlign(VAlign_Center)
                          [
                              SNew(STextBlock)
                              .Text(LOCTEXT("ChooseTextureForThumbnail", "Select the desired texture for the thumbnail assignment"))
                          ]
                      ]
                  ]
                  + SVerticalBox::Slot()
                    .VAlign(VAlign_Top)
                    .AutoHeight()
                  [
                      SNew(SSeparator)
                          .Orientation(Orient_Vertical)
                          .ColorAndOpacity(FLinearColor::Black)
                          .Thickness(2.0f)
                  ]

                  // ASSETS ENTRY ROWS
                  + SVerticalBox::Slot()
                    .VAlign(VAlign_Top)
                    .FillHeight(true)
                  [
                      SAssignNew(AssetScrollBox, SScrollBox)
                      .AnimateWheelScrolling(true)
                  ]

                  // BOTTOM SPACE
                  + SVerticalBox::Slot()
                    .AutoHeight()
                    .Padding(0.0f, 10.0f, 0.0f, 0.0f)
                  [
                      SNew(SBorder)
                      .BorderImage(FCustomThumbnailsStyle::Get().GetBrush("CustomThumbnailEditor.Row.Background"))
                      .Padding(FMargin(10.0f, 0.0f))
                      [

                          SNew(SHorizontalBox)
                          + SHorizontalBox::Slot()
                            .Padding(12.0f)
                            .AutoWidth()
                            .MaxWidth(150.0f)
                          [
                              SNew(SHorizontalBox)
                              + SHorizontalBox::Slot()
                                .HAlign(HAlign_Left)
                                .VAlign(VAlign_Center)
                                .AutoWidth()
                              [
                                  SNew(STextBlock)
                                  .Text(LOCTEXT("OneTextureForAll", "One texture for all assets?"))
                              ]

                              // CHECK BOX
                              + SHorizontalBox::Slot()
                                .HAlign(HAlign_Left)
                                .VAlign(VAlign_Center)
                                .Padding(5.0f, 12.0f)
                                .AutoWidth()
                              [
                                  SNew(SCheckBox)
                                  .OnCheckStateChanged(this, &FCustomThumbnailEditor::OnOneForAllCheckBoxChanged)
                                  .ToolTipText_Raw(this, &FCustomThumbnailEditor::GetTooltipOneForAll)
                                  .IsChecked(ECheckBoxState::Unchecked)
                              ]
                          ]

                          // BUTTONS
                          + SHorizontalBox::Slot()
                            .VAlign(VAlign_Center)
                            .HAlign(HAlign_Right)
                            .FillWidth(1.0f)
                          [
                              SNew(SBox)
                              .HeightOverride(50.0f)
                              .MinDesiredWidth(200.0f)
                              .VAlign(VAlign_Center)
                              .HAlign(HAlign_Right)
                              [
                                  SNew(SHorizontalBox)
                                  + SHorizontalBox::Slot()
                                    .Padding(12.0f)
                                    .HAlign(HAlign_Center)
                                    .VAlign(VAlign_Center)
                                    .AutoWidth()
                                  [
                                      // APPLY
                                      SNew(SButton)
                                      .OnClicked(this, &FCustomThumbnailEditor::OnApply)
                                      .Text(LOCTEXT("ApplyButtonThumbnailPrickerTab", "Apply"))
                                  ]
                                  + SHorizontalBox::Slot()
                                    .Padding(12.0)
                                    .HAlign(HAlign_Center)
                                    .VAlign(VAlign_Center)
                                    .AutoWidth()

                                  [
                                      // CANCEL
                                      SNew(SButton)
                                      .OnClicked(this, &FCustomThumbnailEditor::OnCancel)
                                      .Text(LOCTEXT("CancelButtonThumbnailPickerTab", "Cancel"))
                                  ]
                              ]
                          ]

                      ]
                  ]
              ]
          ];

    Tab->SetBorderBackgroundColor(FLinearColor::Green);

    CreateRowList();

    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FCustomThumbnailsModule::ThumbnailPickerTabName);

    return Tab.ToSharedRef();
}

void FCustomThumbnailEditor::CreateRowList(const bool bReadFromRows)
{
    AssetScrollBox->ClearChildren();

    // Reading the selected assets in the asset property before deleting them, since the set may differ from the original one
    if(bReadFromRows)
    {
        ReadFromRows();
    }

    // Clear ScrollBox before fill
    AssetAndTextureRows.Empty();

    // Fill rows
    for (int32 Index = 0; Index < SelectedAssetsForThumbnail.Num(); ++Index)
    {
        FAssetData TextureAsset = bOneThumbnailForAll && Index == 0
                                      ? (SelectedTextureAssets.IsValidIndex(0) ? SelectedTextureAssets[0] : FAssetData())
                                      : (SelectedTextureAssets.IsValidIndex(Index) ? SelectedTextureAssets[Index] : FAssetData());

        AddAssetRow(SelectedAssetsForThumbnail[Index], TextureAsset);
    }

    AssetScrollBox->AddSlot()
                  .Padding(0.0f, 10.0f)
                  .VAlign(VAlign_Top)
                  .HAlign(HAlign_Center)
    [
        // ADD NEW ASSET ROW BUTTON
        SNew(SBox)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
        [
            SNew(SButton)
                .ButtonStyle(FAppStyle::Get(), "SimpleButton")
                .OnClicked(this, &FCustomThumbnailEditor::OnAddNewAssetRowButtonClicked)
                .DesiredSizeScale(FVector2D(2.0f, 2.0f))
                .ContentPadding(0)
                .Text(LOCTEXT("AddNewRow", "Add new asset row"))
            [
                SNew(SImage)
                    .Image(FEditorStyle::GetBrush("Icons.PlusCircle"))
                    .ColorAndOpacity(FSlateColor::UseForeground())
            ]
        ]
    ];
}

void FCustomThumbnailEditor::AddAssetRow(const FAssetData& Asset, const FAssetData& TextureAsset)
{
    // Check target asset
    if ((Asset.GetAsset() && Asset.GetAsset()->IsA(UTexture2D::StaticClass()))
        || (Asset.GetAsset() && Asset.GetAsset()->IsA(UMaterial::StaticClass())))
        return;

    TSharedPtr<SAssetAndTextureRow> AssetRow;

    // Put new row in scroll box
    AssetScrollBox->AddSlot()
                  .Padding(FMargin(0.0f, 2.0f))
    [
        SAssignNew(AssetRow, SAssetAndTextureRow)
        .SelectedAsset(Asset)
        .SelectedTextureAsset(TextureAsset)
        .OnShouldBeTextureAssetProperty(bOneThumbnailForAll ? AssetAndTextureRows.IsEmpty() : true)
        .OnAssetChanged(this, &FCustomThumbnailEditor::OnAssetChanged)
        .OnShouldFilterAsset(this, &FCustomThumbnailEditor::OnShouldFilterAsset)
        .OnRemoveRow(this, &FCustomThumbnailEditor::OnRemoveRowButtonClicked)
    ];

    // Add asset row in cache array
    AssetAndTextureRows.Add(AssetRow);
}

void FCustomThumbnailEditor::AddAssetRowInExistList(const FAssetData& Asset, const FAssetData& TextureAsset)
{
    AddAssetRow(Asset, TextureAsset);

    CreateRowList();
}

TArray<FAssetData>& FCustomThumbnailEditor::ReadFromRows(const bool bReadEmptyAssets)
{
    if (!AssetAndTextureRows.IsEmpty())
    {
        SelectedAssetsForThumbnail.Empty();
        SelectedTextureAssets.Empty();

        for (TSharedPtr<SAssetAndTextureRow> Row : AssetAndTextureRows)
        {
            TPair<FAssetData, FAssetData> SelectedAssetFromRow = Row->GetSelectedAssets();
            
            const FAssetData Asset = SelectedAssetFromRow.Key;

            if (Asset.IsValid() || bReadEmptyAssets)
            {
                SelectedAssetsForThumbnail.Add(Asset);
            }

            const FAssetData TextureAsset = SelectedAssetFromRow.Value;
            const UTexture2D* TextureObject = Cast<UTexture2D>(TextureAsset.GetAsset());

            if (TextureAsset.IsValid() && TextureObject)
            {
                SelectedTextureAssets.Add(TextureAsset);
            }
            else if (!bOneThumbnailForAll)
            {
                SelectedTextureAssets.Add(FAssetData());
            }
        }
    }

    return SelectedAssetsForThumbnail;
}


FReply FCustomThumbnailEditor::OnApply()
{
    if (!SelectedAssetsForThumbnail.IsEmpty())
    {
        ReadFromRows(false);

        // Capture thumbnails
        FCustomThumbnailAssigner::AssignNewThumbnails(SelectedAssetsForThumbnail, SelectedTextureAssets);

        // Close tab
        Tab->RequestCloseTab();
    }

    return FReply::Handled();
}

FReply FCustomThumbnailEditor::OnCancel()
{
    // Close tab
    Tab->RequestCloseTab();

    return FReply::Handled();
}

FReply FCustomThumbnailEditor::OnAddNewAssetRowButtonClicked()
{
    AddAssetRowInExistList(FAssetData(), FAssetData());

    return FReply::Handled();
}

void FCustomThumbnailEditor::OnCloseTab(TSharedRef<SDockTab> ClosedTab)
{
    TSharedPtr<FCustomThumbnailEditorSummoner> TabSummoner = EditorSummonerPtr.Pin();
    if (TabSummoner.IsValid())
    {
        TabSummoner->OnTabIsClosed(SharedThis(this));
    }
}


void FCustomThumbnailEditor::OnRemoveRowButtonClicked(TSharedRef<SAssetAndTextureRow> RemovedRow)
{
    SelectedAssetsForThumbnail.Remove(RemovedRow->GetSelectedAssets().Key);
    SelectedTextureAssets.Remove(RemovedRow->GetSelectedAssets().Value);
    AssetAndTextureRows.Remove(RemovedRow);

    CreateRowList();
}

FText FCustomThumbnailEditor::GetTooltipOneForAll() const
{
    return FText::FromString("If set, then one thumbnail view will be applied to all assets");
}

void FCustomThumbnailEditor::OnOneForAllCheckBoxChanged(ECheckBoxState CheckBoxState)
{
    switch (CheckBoxState)
    {
        case ECheckBoxState::Unchecked:
        {
            bOneThumbnailForAll = false;

            // If there are cached selected texture assets, then we unload them into the main array
            if (CachedSelectedTextureAssets.Num() > 0)
            {
                for (int32 Index = 0; Index < CachedSelectedTextureAssets.Num(); ++Index)
                {
                    if (SelectedTextureAssets.IsValidIndex(Index))
                    {
                        SelectedTextureAssets[Index] = SelectedTextureAssets[Index].IsValid()
                                                           ? SelectedTextureAssets[Index]
                                                           : CachedSelectedTextureAssets[Index];
                    }
                    else
                    {
                        SelectedTextureAssets.Add(CachedSelectedTextureAssets[Index]);
                    }
                }
            }
            break;
        }
        case ECheckBoxState::Checked:
        {
            bOneThumbnailForAll = true;
            
            // Caching the selected texture assets
            CachedSelectedTextureAssets.Empty();
            for(FAssetData& SelectedTextureAsset : SelectedTextureAssets)
            {
                CachedSelectedTextureAssets.Add(SelectedTextureAsset);
            }
            break;
        }
        case ECheckBoxState::Undetermined: break;
        default: ;
    }
    
    CreateRowList(false);
}

bool FCustomThumbnailEditor::OnShouldFilterAsset(const FAssetData& AssetData) const
{
    return SelectedAssetsForThumbnail.Contains(AssetData);
}

void FCustomThumbnailEditor::OnAssetChanged(const FAssetData& AssetData)
{
    ReadFromRows();
}


#undef LOCTEXT_NAMESPACE
