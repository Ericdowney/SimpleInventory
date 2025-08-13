// Copyright Eric Downey - 2025

#include "SimpleInventoryTests.h"

#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogSimpleInventoryTests, Log, All);

void FSimpleInventoryTestsModule::StartupModule()
{
    UE_LOG(LogSimpleInventoryTests, Log, TEXT("SimpleInventoryTests: StartupModule called"));
}

void FSimpleInventoryTestsModule::ShutdownModule()
{
    UE_LOG(LogSimpleInventoryTests, Log, TEXT("SimpleInventoryTests: ShutdownModule called"));
}

IMPLEMENT_MODULE(FSimpleInventoryTestsModule, SimpleInventoryTests)
