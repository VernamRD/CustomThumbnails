//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "ICustomThumbnailsModuleInterface.h"

/*=====================================================================================================================
*                              How does it work?
*======================================================================================================================
*
*   1. Unreal Engine creates an instance of the module and calls StartupModule through the IModuleInterface
*   
*   2. Static CustomThumbnailsStyle methods are called to initialize module style: icons, path to module resources and content, etc.
*
*   FCustomThumbnailsModule::CustomExtender subscribes to the ContentBrowserModule Asset Selection Delegate 
*
*   FCustomThumbnailsModule::CustomExtender creates an extender, an instance of CustomThumbnailPickerTab, ContentBrowserMenuExtension.
*   The CustomThumbnailPickerTab constructor registers a new tab ready to be called in FGlobalTabmanager
*   FContentBrowserMenuExtension::AddMenuEntry subscribes to the AddMenuExtension delegate
*
*   5. The FContentBrowserMenuExtension::AddMenuEntry initializes additional Asset action menu items.
*   Then it waits for the Callback to click on the created item.
*   When you click through the FGlobalTabmanager,
*   an attempt is made to create a tab of the "CustomThumbnailPickerTab" module.
*
*   6. FGlobalTabmanager calls the FCustomThumbnailPickerTab::OnSpawnTab method, which contains the tab design
*   In FCustomThumbnailPickerTab, PropertyEntryBoxes are created in which the user selects the necessary assets,
*   as well as exit and apply buttons.
*
*   The property boxes are created by the FCustomThumbnailPickerTab::CreateEntryBoxList function,
*   which creates custom storage-operator instances and places them in an array:
*   
*   *CustomAssetProperty
*   *TextureAssetProperty
*
*   These stores have a similar functionality to the usual SObjectPropertyEntryBox,
*   but allow more convenient to keep them in an array, read the data, or maybe I'm just a crook
*
*   7. When you click the apply button, the Callback function FCustomThumbnailPickerTab::OnApply comes out,
*   which reads the assets in the proprietary boxes and sends them to the static method CustomCapturerThumbnail::CaptureNewCustomThumbnails
*
*   CustomCapturerThumbnail::CaptureNewCustomThumbnails Gets the thumbnail used for the asset textures, and applies it to the corresponding asset. 
*   The thumbnail is cached, so changing the texture will not change the thumbnails of the asset that uses it.
*
*====================================================================================================================*/

class FSlateStyleSet;
class FCustomThumbnailPickerTab;
class FContentBrowserMenuExtension;

DECLARE_LOG_CATEGORY_EXTERN(LogCTModule, All, All);

class CUSTOMTHUMBNAILS_API FCustomThumbnailsModule : public ICustomThumbnailsModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /** Checking the array of pointers, as well as their relevance to module tabs */
    void CheckPointers();
    
    /** Function for module elements to get the tab name
     *  @return Tab name
     */
    FName GetThumbnailPickerTabName() const { return ThumbnailPickerTabName; }

    /** Create extender and init extend asset menu in ContentBrowserMenuExtension     
     * @param Assets Selected assets
     * @return MenuExtender
    */
    TSharedRef<FExtender> CustomExtender(const TArray<FAssetData>& Assets);
    TSharedPtr<FContentBrowserMenuExtension> Extension;

protected:
    /** Array storing all tabs of the module */
    TArray<TSharedPtr<FCustomThumbnailPickerTab>> PickerTabs;

    /** Stores the style set for this module*/
    TSharedPtr<class FSlateStyleSet> StyleSet;
    
    /** For which classes you need to redefine the renderer */
    TArray<UClass*> RendererTargets;

    /** Array of Class and Icon Correspondence */
    TMap<UClass*, UTexture2D*> ClassThumbnail;

    /** Tab name */
    const FName ThumbnailPickerTabName = "ThumbnailPicker";
};
