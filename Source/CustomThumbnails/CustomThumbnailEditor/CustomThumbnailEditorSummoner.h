// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FCustomThumbnailEditor;

class FCustomThumbnailEditorSummoner : public TSharedFromThis<FCustomThumbnailEditorSummoner>
{
public:
    FCustomThumbnailEditorSummoner();

    void CreateEditor(const TArray<FAssetData>& Assets);
    void OnTabIsClosed(TSharedPtr<FCustomThumbnailEditor> ClosedTab);

protected:
    /** Array storing all tabs*/
    TArray<TSharedPtr<FCustomThumbnailEditor>> CustomThumbnailsEditors;
};
