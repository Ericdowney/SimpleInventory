// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "SimpleInventoryItem.h"

#include "SimpleInventorySlot.generated.h"

/**
 * Object to hold `FSimpleInventoryItem` and the amount "stacked" as a single slot.
 */
UCLASS(ClassGroup=(SimpleInventory), BlueprintType, Blueprintable)
class SIMPLEINVENTORY_API USimpleInventorySlot : public UObject
{
    GENERATED_BODY()

public:
    USimpleInventorySlot();
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Slot")
    FInstancedStruct Item;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Slot")
    int32 Count;
};
