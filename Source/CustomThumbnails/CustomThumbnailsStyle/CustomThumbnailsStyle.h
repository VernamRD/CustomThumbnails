//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FCustomThumbnailsStyle
{
public:
    static void Initialize();

    static void Shutdown();

    static void ReloadTextures();

    static const ISlateStyle& Get();

    static FName GetStyleSetName();

private:
    static TSharedRef<FSlateStyleSet> Create();

    static TSharedPtr<FSlateStyleSet> StyleInstance;
};
