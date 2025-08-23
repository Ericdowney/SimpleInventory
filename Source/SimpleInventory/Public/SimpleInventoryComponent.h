// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "Components/ActorComponent.h"

#include "SimpleInventoryComponent.generated.h"

class USimpleInventorySlot;
class USimpleInventory;

UCLASS(ClassGroup=(SimpleInventory), meta=(BlueprintSpawnableComponent))
class SIMPLEINVENTORY_API USimpleInventoryComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="Simple Inventory Component")
    USimpleInventory* Inventory;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="Simple Inventory Component")
    int32 MaxSlotSize;
    
    USimpleInventoryComponent();
    
    /**
     * Add an item to this inventory.
     *
     * @param Item    The item to add.
     * @param Count   The number of items to add.
     * @param Result  True if the item(s) were successfully added.
     */
    void AddItem(FInstancedStruct Item,
                 const int32 Count,
                 bool& Result);
    
    /**
     * Remove a number of items from a specific slot.
     *
     * @param Index   The index of the slot to remove from.
     * @param Count   How many items to remove.
     * @param Result  True if the removal was successful.
     */
    void RemoveItemAtIndex(const int32 Index,
                           const int32 Count,
                           bool& Result);
    
    /**
    * Remove one instance of each item in the given list.
    *
    * @param Items   The items to remove (one count each).
    * @param Result  True if all items were successfully removed.
    */
    void RemoveItems(const TArray<FInstancedStruct>& Items,
                     bool& Result);
    
    /**
     * Clear all items from this inventory.
     */
    void Clear();
    
    /**
     * Get the current number of occupied slots.
     *
     * @param Result  Outputs the number of used slots.
     */
    void GetLength(int32& Result) const;
    
    /**
     * Get the maximum number of slots available in this inventory.
     *
     * @param Result  Outputs the maximum slot capacity.
     */
    void GetMaxSize(int32& Result) const;
    
    /**
     * Get a slot by index.
     *
     * @param Index   The index of the slot.
     * @param Result  The inventory slot at the given index, or nullptr if invalid.
     */
    void GetSlot(const int32 Index,
                 USimpleInventorySlot*& Result) const;
    
    /**
     * Get all slots in this inventory.
     *
     * @param Result  Array of inventory slots.
     */
    void GetSlots(TArray<USimpleInventorySlot*>& Result) const;
    
    /**
     * Copy the contents of another inventory into this one.
     *
     * @param OtherInventory  The inventory to copy from.
     */
    void CopyInventory(const USimpleInventory* OtherInventory);
    
    /**
     * Force this inventory to broadcast a change event.
     * Useful for refreshing UI without actual changes.
     */
    void ForceOnChange() const;
    
protected:
    void BeginPlay() override;
};
