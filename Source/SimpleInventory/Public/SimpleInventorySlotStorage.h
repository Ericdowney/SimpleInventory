// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StructUtils/InstancedStruct.h"

#include "SimpleInventorySlotStorage.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct SIMPLEINVENTORY_API FSimpleInventorySlotStorage
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Slot Storage")
    FInstancedStruct Metadata;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Slot Storage")
    int32 Count = 0;
};
