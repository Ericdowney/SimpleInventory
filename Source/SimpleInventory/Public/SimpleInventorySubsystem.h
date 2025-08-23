// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SimpleInventoryStorage.h"
#include "SimpleInventorySlotStorage.h"
#include "SimpleInventorySubsystemStorage.h"

#include "SimpleInventorySubsystem.generated.h"

class USimpleInventory;
class USimpleInventoryDefinitions;
class USimpleInventoryChange;

UCLASS(ClassGroup=(SimpleInventory), Blueprintable, BlueprintType)
class SIMPLEINVENTORY_API USimpleInventorySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
    
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimpleInventorySubsystemChangeDelegate, USimpleInventoryChange*, InventoryChange);
    
    UPROPERTY(BlueprintAssignable, Category="Simple Inventory Subsystem")
    FOnSimpleInventorySubsystemChangeDelegate OnInventorySubsystemChangeEvent;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Subsystem")
    TMap<FName, TObjectPtr<USimpleInventory>> InventoryMap;
    
    void Initialize(FSubsystemCollectionBase& Collection) override;
    
    /**
     * Get all registered inventories managed by this subsystem.
     *
     * @param Result  Output map of inventory names to inventory instances.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetAllInventories(TMap<FName, USimpleInventory*>& Result);
    
    /**
     * Get a specific inventory by name.
     *
     * @param InventoryName  The name of the inventory to retrieve.
     * @param Result         The inventory instance, or nullptr if not found.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetInventory(const FName InventoryName,
                      USimpleInventory*& Result);
    
    /**
     * Get the number of occupied slots in a given inventory.
     *
     * @param InventoryName  The name of the inventory to check.
     * @param Result         The current number of slots used.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetLength(const FName InventoryName,
                   int32& Result);
    
    /**
     * Get the maximum number of slots available for a given inventory.
     *
     * @param InventoryName  The name of the inventory to check.
     * @param Result         The maximum slot capacity.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetMaxSize(const FName InventoryName,
                    int32& Result);
    
    /**
     * Get a single inventory slot by index.
     *
     * @param InventoryName  The name of the inventory to access.
     * @param Index          The slot index to retrieve.
     * @param Result         The inventory slot at the given index, or nullptr if invalid.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetSlot(const FName InventoryName,
                 const int32 Index,
                 USimpleInventorySlot*& Result);
    
    /**
     * Get all slots from the specified inventory.
     *
     * @param InventoryName  The name of the inventory to access.
     * @param Result         Array of inventory slots.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetSlots(const FName InventoryName,
                  TArray<USimpleInventorySlot*>& Result);
    
    /**
     * Serialize all inventories managed by the subsystem into a storage struct.
     *
     * @param Result  Output storage structure representing the current state.
     */
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetStorage(FSimpleInventorySubsystemStorage& Result) const;
    
    /**
     * Check if an inventory contains an item with an exact count.
     *
     * @param InventoryName  The name of the inventory to check.
     * @param ItemID         The ID of the item to search for.
     * @param Count          The exact quantity required.
     * @param Result         True if the inventory contains the item with the given count.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void HasItem(const FName InventoryName,
                 const int32 ItemID,
                 const int32 Count,
                 bool& Result);
    
    /**
     * Register multiple inventories from a Data Asset.
     *
     * @param Definitions  Data asset containing one or more inventory definitions.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void RegisterInventoryDefinitions(USimpleInventoryDefinitions* Definitions);

    /**
     * Register a single inventory with the given name and slot capacity.
     * If an inventory with the same name already exists, the existing one is returned.
     *
     * @param InventoryName  The name of the new inventory.
     * @param MaxSlots       The maximum number of slots.
     * @param Result         The registered or existing inventory.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void RegisterInventory(const FName InventoryName,
                           const int32 MaxSlots,
                           USimpleInventory*& Result);
    
    /**
     * Add an item to the specified inventory.
     *
     * @param InventoryName  The name of the inventory to modify.
     * @param Item           The item to add.
     * @param Count          The number of items to add.
     * @param Result         True if the item(s) were successfully added.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void AddItem(const FName InventoryName,
                 FInstancedStruct Item,
                 const int32 Count,
                 bool& Result);
    
    /**
     * Remove a quantity of an item at a specific index.
     *
     * @param InventoryName  The name of the inventory to modify.
     * @param Index          The slot index to remove from.
     * @param Count          How many items to remove.
     * @param Result         True if removal was successful.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void RemoveItemAtIndex(const FName InventoryName,
                           const int32 Index,
                           const int32 Count,
                           bool& Result);
    
    /**
     * Remove one instance of each item in the given list.
     *
     * @param InventoryName  The name of the inventory to modify.
     * @param Items          The items to remove (one count each).
     * @param Result         True if all items were removed.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void RemoveItems(const FName InventoryName,
                     const TArray<FInstancedStruct>& Items,
                     bool& Result);
    
    /**
     * Clear all items from a given inventory.
     *
     * @param InventoryName  The name of the inventory to clear.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void Clear(const FName InventoryName);
    
    /**
     * Clear all inventories registered in the subsystem.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void ClearAll();
    
    /**
     * Copy the contents of another inventory into the target inventory.
     *
     * @param InventoryName   The name of the inventory to copy into.
     * @param OtherInventory  The source inventory.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void CopyInventory(const FName InventoryName,
                       const USimpleInventory* OtherInventory);
    
    /**
     * Force an inventory to broadcast a change event.
     * Useful for UI refreshes when no actual change occurred.
     *
     * @param InventoryName  The name of the inventory to force update.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void ForceOnChange(const FName InventoryName) const;
    
    /**
     * Restore inventories from a serialized storage struct.
     *
     * @param Storage  Struct containing saved inventory data.
     */
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void InflateFromStorage(const FSimpleInventorySubsystemStorage Storage);
    
private:
    UFUNCTION()
    void Find(const FName InventoryName,
              USimpleInventory*& Result) const;
    
    UFUNCTION()
    void HandleOnChangeEvent(USimpleInventoryChange* InventoryChange);
};
