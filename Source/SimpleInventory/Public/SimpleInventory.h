// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "SimpleInventory.generated.h"

class USimpleInventorySlot;
class USimpleInventoryChange;

UCLASS(ClassGroup=(SimpleInventory), BlueprintType, Blueprintable)
class SIMPLEINVENTORY_API USimpleInventory : public UObject
{
    GENERATED_BODY()
    
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChangeDelegate, USimpleInventoryChange*, InventoryChange);
    
    UPROPERTY(BlueprintAssignable, Category="Simple Inventory")
    FOnInventoryChangeDelegate OnInventoryChangeEvent;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="Simple Inventory")
    FName InventoryName;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="Simple Inventory")
    int32 MaxSlotSize;
    
    USimpleInventory();
    
    /**
     * Add an item to the inventory.
     * If the item is stackable, it will be merged into an existing stack when possible.
     * Otherwise, it will be placed into a new slot if space is available.
     *
     * @param Item    The item to add.
     * @param Count   The number of items to add.
     * @param Result  True if the item(s) were successfully added.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void AddItem(FInstancedStruct Item,
                 const int32 Count,
                 bool& Result);
    
    /**
     * Remove a number of items from a specific slot.
     * If the slot’s count reaches zero, the slot will be removed.
     *
     * @param Index   The index of the slot to remove from.
     * @param Count   How many items to remove.
     * @param Result  True if the removal was successful.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void RemoveItemAtIndex(const int32 Index,
                           const int32 Count,
                           bool& Result);
    
    /**
     * Remove one instance of each item in the given list.
     *
     * @param Items   The items to remove (one count each).
     * @param Result  True if all items were successfully removed.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void RemoveItems(const TArray<FInstancedStruct>& Items,
                     bool& Result);
    
    /**
     * Clear all items from the inventory.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void Clear();
    
    /**
     * Get the number of occupied slots in the inventory.
     *
     * @param Result  Outputs the number of used slots.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetLength(int32& Result);
    
    /**
     * Get the maximum number of slots this inventory can hold.
     *
     * @param Result  Outputs the maximum slot capacity.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetMaxSize(int32& Result);
    
    /**
     * Get a specific slot by index.
     *
     * @param Index   The index of the slot.
     * @param Result  The inventory slot at the given index, or nullptr if invalid.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetSlot(const int32 Index,
                 USimpleInventorySlot*& Result) const;
    
    /**
     * Get all slots in the inventory.
     *
     * @param Result  Array of inventory slots.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetSlots(TArray<USimpleInventorySlot*>& Result) const;
    
    /**
     * Check if the inventory contains a specific item with an exact count.
     *
     * @param ItemID  The ID of the item to search for.
     * @param Count   The exact quantity required.
     * @param Result  True if the item exists with the specified count.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void HasItem(const int32 ItemID,
                 const int32 Count,
                 bool& Result);
    
    /**
     * Copy the contents of another inventory into this one.
     *
     * @param OtherInventory  The inventory to copy from.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void CopyInventory(const USimpleInventory* OtherInventory);
    
    /**
     * Force the inventory to broadcast a change event.
     * Useful for refreshing UI without actual changes.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void ForceOnChange() const;

    /**
     * Force the internal slots array to resize to the maximum slot size.
     * Useful for ensuring UI layouts or saving systems match the configured size.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void ForceResize();
    
protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="Simple Inventory")
    TArray<USimpleInventorySlot*> InventorySlots;
    
private:
    
    void AddItemToNewSlot(FInstancedStruct Item,
                          const int32 Count);
};
