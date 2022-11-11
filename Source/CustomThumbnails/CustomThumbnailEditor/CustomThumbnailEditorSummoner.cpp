// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomThumbnailEditorSummoner.h"

#include "CustomThumbnailEditor.h"
#include "CustomThumbnails/CustomThumbnails.h"

FCustomThumbnailEditorSummoner::FCustomThumbnailEditorSummoner() { }

void FCustomThumbnailEditorSummoner::CreateEditor(const TArray<FAssetData>& Assets)
{
    CustomThumbnailsEditors.Add(MakeShareable(new FCustomThumbnailEditor(AsWeak(), Assets)));
}

void FCustomThumbnailEditorSummoner::OnTabIsClosed(TSharedPtr<FCustomThumbnailEditor> ClosedTab)
{
    check(ClosedTab.IsValid() && CustomThumbnailsEditors.FindByKey(ClosedTab));
    CustomThumbnailsEditors.Remove(ClosedTab);
}
