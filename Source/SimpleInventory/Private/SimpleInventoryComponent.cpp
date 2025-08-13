// Copyright Eric Downey - 2025

#include "SimpleInventoryComponent.h"

#include "SimpleInventory.h"
#include "SimpleInventoryLog.h"

// Lifecycle

/**
 * Constructor for the Simple Inventory Component.
 * Disables ticking, as this component does not need per-frame updates.
 */
USimpleInventoryComponent::USimpleInventoryComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

/**
 * Called when the game starts or when the component is spawned.
 * Sets the maximum inventory size and ensures the inventory is properly resized.
 */
void USimpleInventoryComponent::BeginPlay() {
    Super::BeginPlay();
    
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::BeginPlay"));
    Inventory->MaxSlotSize = MaxSlotSize;
    Inventory->ForceResize();
}

/**
 * Adds an item to the inventory.
 *
 * @param Item   The item to add (as an instanced struct).
 * @param Count  The number of items to add.
 * @param Result True if the item was successfully added, false otherwise.
 */
void USimpleInventoryComponent::AddItem(FInstancedStruct Item,
                                        const int32 Count,
                                        bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::AddItem || Count: %i"), Count);
    
    Inventory->AddItem(Item, Count, Result);
}

/**
 * Removes a specified number of items at a given index.
 *
 * @param Index  The slot index to remove items from.
 * @param Count  The number of items to remove.
 * @param Result True if the items were successfully removed, false otherwise.
 */
void USimpleInventoryComponent::RemoveItemAtIndex(const int32 Index,
                                                  const int32 Count,
                                                  bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::RemoveItemAtIndex || Index: %i | Count: %i"), Index, Count);
    
    Inventory->RemoveItemAtIndex(Index, Count, Result);
}

/**
 * Removes all matching items from the inventory.
 *
 * @param Items  An array of items to remove.
 * @param Result True if all items were successfully removed, false otherwise.
 */
void USimpleInventoryComponent::RemoveItems(const TArray<FInstancedStruct>& Items,
                                            bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::RemoveItems"));
    
    Inventory->RemoveItems(Items, Result);
}

/**
 * Clears all items from the inventory.
 */
void USimpleInventoryComponent::Clear() {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::Clear"));
    
    Inventory->Clear();
}

/**
 * Gets the current number of occupied slots in the inventory.
 *
 * @param Result Outputs the current slot count.
 */
void USimpleInventoryComponent::GetLength(int32& Result) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::GetLength"));
    
    Inventory->GetLength(Result);
}

/**
 * Gets the maximum number of slots the inventory can hold.
 *
 * @param Result Outputs the maximum slot capacity.
 */
void USimpleInventoryComponent::GetMaxSize(int32& Result) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::GetMaxSize"));
    
    Inventory->GetMaxSize(Result);
}

/**
 * Retrieves a reference to a specific inventory slot by index.
 *
 * @param Index  The index of the slot to retrieve.
 * @param Result Outputs the pointer to the inventory slot.
 */
void USimpleInventoryComponent::GetSlot(const int32 Index,
                                        USimpleInventorySlot*& Result) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::GetSlot || Index: %i"), Index);
    
    Inventory->GetSlot(Index, Result);
}

/**
 * Retrieves all inventory slots.
 *
 * @param Result Outputs an array of pointers to all inventory slots.
 */
void USimpleInventoryComponent::GetSlots(TArray<USimpleInventorySlot*>& Result) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::GetSlots"));
    
    Inventory->GetSlots(Result);
}

/**
 * Copies the contents of another inventory into this one.
 *
 * @param OtherInventory The inventory to copy from.
 */
void USimpleInventoryComponent::CopyInventory(const USimpleInventory* OtherInventory) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::CopyInventory"));
    
    Inventory->CopyInventory(OtherInventory);
}

/**
 * Forces the inventory to fire its change event, useful for UI updates.
 */
void USimpleInventoryComponent::ForceOnChange() const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventoryComponent::ForceOnChange"));
    
    Inventory->ForceOnChange();
}
