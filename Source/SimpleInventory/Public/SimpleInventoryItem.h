// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"

#include "SimpleInventoryItem.generated.h"

/**
 * Foundational structure for in-game items that can be stored in a `USimpleInventory`.
 */
USTRUCT(BlueprintType, Blueprintable)
struct SIMPLEINVENTORY_API FSimpleInventoryItem : public FTableRowBase
{
    GENERATED_BODY()

public:
    /** Inventory Item Identifier */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Item")
    int32 ID = 0;
    
    /** Determines whether the item is stackable or not in Inventory Slots. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Item")
    bool bIsStackable = false;
    
    /** The amount of items allowed to be stacked in Inventory Slots, if `bIsStackable` is `true`. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Item")
    int32 StackSize = 0;

    /** The name of the item to be displayed to the player. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Item")
    FText ItemName;
};
