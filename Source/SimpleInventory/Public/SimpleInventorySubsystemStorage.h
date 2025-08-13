// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SimpleInventoryStorage.h"

#include "SimpleInventorySubsystemStorage.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct SIMPLEINVENTORY_API FSimpleInventorySubsystemStorage
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Subsystem Storage")
    TMap<FName, FSimpleInventoryStorage> Value;
};
