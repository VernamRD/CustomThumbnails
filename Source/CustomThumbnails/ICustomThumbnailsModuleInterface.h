//Copyright © 2022 TIGan. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FCustomThumbnailsModule;

class ICustomThumbnailsModuleInterface : public IModuleInterface
{
public:
    virtual void StartupModule() override { }

    virtual void ShutdownModule() override { }
};
