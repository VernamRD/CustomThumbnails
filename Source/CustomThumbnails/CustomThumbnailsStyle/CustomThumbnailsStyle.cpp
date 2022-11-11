//Copyright © 2022 TIGan. All Rights Reserved.

#include "CustomThumbnailsStyle.h"

#include "CustomThumbnails/CustomThumbnails.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define TAB_SIZE FVector2D(900.0f, 700.0f)
#define STYLE_NAME "CustomThumbnailsStyle"

DEFINE_LOG_CATEGORY_STATIC(LogCTStyle, All, All);

TSharedPtr<FSlateStyleSet> FCustomThumbnailsStyle::StyleInstance = nullptr;

void FCustomThumbnailsStyle::Initialize()
{
    UE_LOG(LogCTStyle, Verbose, TEXT("Initialize"));

    if (!StyleInstance.IsValid())
    {
        UE_LOG(LogCTStyle, VeryVerbose, TEXT("Initialize StyleInstance"));

        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);

        // Set default tab size
        const FCustomThumbnailsModule& CTModule = FModuleManager::GetModuleChecked<FCustomThumbnailsModule>(TEXT("CustomThumbnails"));
        FGlobalTabmanager::Get()->RegisterDefaultTabWindowSize(CTModule.ThumbnailPickerTabName, TAB_SIZE);
    }
}

void FCustomThumbnailsStyle::Shutdown()
{
    UE_LOG(LogCTStyle, Verbose, TEXT("Shutdown"));

    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

FName FCustomThumbnailsStyle::GetStyleSetName()
{
    static FName StyleSetName(TEXT(STYLE_NAME));
    return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16X16(16.0f, 16.0f);
const FVector2D Icon20X20(20.0f, 20.0f);
const FVector2D Icon40X40(40.0f, 40.0f);

TSharedRef<FSlateStyleSet> FCustomThumbnailsStyle::Create()
{
    UE_LOG(LogCTStyle, VeryVerbose, TEXT("Create"));

    // Create SlateStyle
    TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(STYLE_NAME));
    Style->SetParentStyleName("EditorStyle");

    // Set Content/Resources root for module
    Style->SetContentRoot(IPluginManager::Get().FindPlugin("CustomThumbnails")->GetBaseDir() / TEXT("Resources"));
    
    // Set path for icons
    Style->Set("ContentBrowser.AssetAction.AddThumbnail", new IMAGE_BRUSH(TEXT("AssetActionAddButtonIcon_16X"), Icon40X40));
    Style->Set("ContentBrowser.AssetAction.ClearThumbnail", new IMAGE_BRUSH(TEXT("AssetActionClearButtonIcon_16X"), Icon40X40));

    Style->Set("CustomThumbnailEditor.Background", FLinearColor(0.01f, 0.01f, 0.01f, 1.0f));
    FSlateBrush* LocalEditorBackgroundBrush = new FSlateBrush();
    LocalEditorBackgroundBrush->TintColor = Style->GetColor("CustomThumbnailEditor.Background");
    Style->Set("CustomThumbnailEditor.Background", MoveTemp(LocalEditorBackgroundBrush));

    Style->Set("CustomThumbnailEditor.Row.Background", FLinearColor(0.02f, 0.02f, 0.02f, 1.0f));
    FSlateBrush* LocalRowBackgroundBrush = new FSlateBrush();
    LocalRowBackgroundBrush->TintColor = Style->GetColor("CustomThumbnailEditor.Row.Background");
    Style->Set("CustomThumbnailEditor.Row.Background", MoveTemp(LocalRowBackgroundBrush));

    return Style;
}

#undef TAB_SIZE
#undef STYLE_NAME

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FCustomThumbnailsStyle::ReloadTextures()
{
    if (FSlateApplication::IsInitialized())
    {
        UE_LOG(LogCTStyle, VeryVerbose, TEXT("RelaodTexture"));

        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}

const ISlateStyle& FCustomThumbnailsStyle::Get()
{
    return *StyleInstance;
}
