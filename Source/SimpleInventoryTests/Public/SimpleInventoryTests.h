// Copyright Eric Downey - 2025

#pragma once

#include "Modules/ModuleManager.h"

class FSimpleInventoryTestsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
