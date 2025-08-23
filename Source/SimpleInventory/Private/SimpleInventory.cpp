// Copyright Eric Downey - 2025

#include "SimpleInventory.h"

#include "SimpleInventorySlot.h"
#include "SimpleInventoryLog.h"
#include "SimpleInventoryChange.h"
#include "SimpleInventoryChangeType.h"

// Lifecycle

/**
 * Constructor for USimpleInventory.
 * Reserves memory for inventory slots up to MaxSlotSize to optimize performance.
 */
USimpleInventory::USimpleInventory() {
    InventorySlots.Reserve(MaxSlotSize);
}

// Public Functions

/**
 * Adds an item to the inventory. If the item is stackable and an existing stack can hold more, it is added to that stack.
 * Otherwise, it attempts to create a new slot for the item, if space permits.
 *
 * @param Item    The item to add, wrapped in an FInstancedStruct.
 * @param Count   The number of items to add.
 * @param Result  True if the item(s) were added successfully, false otherwise.
 */
void USimpleInventory::AddItem(FInstancedStruct Item,
                               const int32 Count,
                               bool& Result)  {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::AddItem"));
    
    Result = false;
    if (!Item.IsValid()) {
        UE_LOG(SimpleInventoryLog, Warning, TEXT("USimpleInventory::AddItem || Invalid InstancedStruct"));
        return;
    }
    
    static const FName IDPropName = TEXT("ID");
    static const FName IsStackablePropName = TEXT("bIsStackable");
    static const FName StackSizePropName = TEXT("StackSize");
    
    const UStruct* ItemStructType = Item.GetScriptStruct();
    void* ItemStructMemory = Item.GetMutableMemory();
    if (!ItemStructType || !ItemStructMemory) {
        UE_LOG(SimpleInventoryLog, Warning, TEXT("USimpleInventory::AddItem || Struct type or memory is null"));
        return;
    }
    
    // Get the ItemID from the struct
    FProperty* ItemIDProp = ItemStructType->FindPropertyByName(IDPropName);
    FIntProperty* ItemIDIntProp = CastField<FIntProperty>(ItemIDProp);
    if (!ItemIDIntProp) {
        UE_LOG(SimpleInventoryLog, Warning, TEXT("USimpleInventory::AddItem || Struct missing valid 'ID' int property"));
        return;
    }
    const int32 ItemID = ItemIDIntProp->GetPropertyValue_InContainer(ItemStructMemory);
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::AddItem || ItemID = %d"), ItemID);
    
    int32 Remaining = Count;
    
    // --- Pass 1: Fill existing stacks ---
    for (USimpleInventorySlot* iSlot : InventorySlots) {
        if (!iSlot || !iSlot->Item.IsValid() || Remaining <= 0) {
            continue;
        }
        
        const UStruct* SlotStruct = iSlot->Item.GetScriptStruct();
        void* SlotMemory = iSlot->Item.GetMutableMemory();
        if (!SlotStruct || !SlotMemory) {
            continue;
        }
        
        FIntProperty* SlotIDProp = CastField<FIntProperty>(SlotStruct->FindPropertyByName(IDPropName));
        FBoolProperty* StackableProp = CastField<FBoolProperty>(SlotStruct->FindPropertyByName(IsStackablePropName));
        FIntProperty* StackSizeProp = CastField<FIntProperty>(SlotStruct->FindPropertyByName(StackSizePropName));
        
        if (!SlotIDProp || !StackableProp || !StackSizeProp) {
            UE_LOG(SimpleInventoryLog, Warning, TEXT("USimpleInventory::AddItem || Slot missing expected properties"));
            continue;
        }
        
        const int32 SlotItemID = SlotIDProp->GetPropertyValue_InContainer(SlotMemory);
        const bool bStackable = StackableProp->GetPropertyValue_InContainer(SlotMemory);
        const int32 MaxStackSize = StackSizeProp->GetPropertyValue_InContainer(SlotMemory);
        
        if (SlotItemID == ItemID && bStackable) {
            int32 SpaceLeft = MaxStackSize - iSlot->Count;
            if (SpaceLeft > 0) {
                int32 ToAdd = FMath::Min(SpaceLeft, Remaining);
                iSlot->Count += ToAdd;
                Remaining -= ToAdd;
                
                USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
                Change->InventoryName = InventoryName;
                Change->Type = ESimpleInventoryChangeType::ADDITION;
                Change->Item = Item;
                Change->Count = ToAdd;
                OnInventoryChangeEvent.Broadcast(Change);
                
                UE_LOG(SimpleInventoryLog, Log, TEXT("USimpleInventory::AddItem || Added %d items to existing stack"), ToAdd);
            }
        }
    }
    
    // --- Pass 2: Create new stacks ---
    while (Remaining > 0 && InventorySlots.Num() < MaxSlotSize) {
        const int32 ToAdd = Remaining;
        
        AddItemToNewSlot(Item, ToAdd);
        
        USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
        Change->InventoryName = InventoryName;
        Change->Type = ESimpleInventoryChangeType::ADDITION;
        Change->Item = Item;
        Change->Count = ToAdd;
        OnInventoryChangeEvent.Broadcast(Change);
        
        UE_LOG(SimpleInventoryLog, Log, TEXT("USimpleInventory::AddItem || Added %d items to new slot"), ToAdd);
        
        Remaining -= ToAdd;
    }
    
    Result = (Remaining == 0);
    
    if (!Result) {
        UE_LOG(SimpleInventoryLog, Warning, TEXT("USimpleInventory::AddItem || Inventory full, %d items could not be added"), Remaining);
        
        USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
        Change->InventoryName = InventoryName;
        Change->Type = ESimpleInventoryChangeType::FULL;
        OnInventoryChangeEvent.Broadcast(Change);
    }
}

/**
 * Removes a specified number of items from a given index in the inventory.
 * If the count drops to zero or below, the slot is removed entirely.
 *
 * @param Index   Index of the inventory slot to remove from.
 * @param Count   Number of items to remove from the slot.
 * @param Result  True if the removal was successful, false if the index was invalid.
 */
void USimpleInventory::RemoveItemAtIndex(const int32 Index,
                                         const int32 Count,
                                         bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::RemoveItemAtIndex || Index: %i | Count: %i"), Index, Count);
    
    if (!InventorySlots.IsValidIndex(Index)) {
        Result = false;
        return;
    }
    
    USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
    Change->InventoryName = InventoryName;
    Change->Type = ESimpleInventoryChangeType::REMOVAL;
    
    USimpleInventorySlot *Slot = InventorySlots[Index];
    Change->Type = ESimpleInventoryChangeType::REMOVAL;
    Change->Item = Slot->Item;
    Slot->Count -= Count;
    
    if (Slot->Count <= 0) {
        InventorySlots.RemoveAt(Index);
        Change->Count = 0;
    }
    else {
        Change->Count = Slot->Count;
    }
    OnInventoryChangeEvent.Broadcast(Change);
    
    Result = true;
}

/**
 * Removes one instance of each item in the given array from the inventory.
 * Matches items based on their "ID" property.
 *
 * @param Items   Array of items to remove (one count each).
 * @param Result  True if all items were successfully removed, false otherwise.
 */
void USimpleInventory::RemoveItems(const TArray<FInstancedStruct>& Items,
                                   bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::RemoveItems"));
    
    const FName IDPropName = FName("ID");
    
    uint8 RemainingCount = Items.Num();
    for (FInstancedStruct Item : Items) {
        const UStruct* ItemStructType = Item.GetScriptStruct();
        void* ItemStructMemory = Item.GetMutableMemory();
        
        FProperty* ItemIDProp = ItemStructType->FindPropertyByName(IDPropName);
        FIntProperty* ItemIDIntProp = CastField<FIntProperty>(ItemIDProp);
        int32 ItemID = ItemIDIntProp->GetPropertyValue_InContainer(ItemStructMemory);
        
        for (uint8 Index = 0; Index < InventorySlots.Num(); ++Index) {
            USimpleInventorySlot* Slot = InventorySlots[Index];
            const UStruct* SlotItemStructType = Slot->Item.GetScriptStruct();
            void* SlotItemStructMemory = Slot->Item.GetMutableMemory();
            
            FProperty* SlotItemIDProp = SlotItemStructType->FindPropertyByName(IDPropName);
            FIntProperty* SlotItemIDIntProp = CastField<FIntProperty>(SlotItemIDProp);
            int32 SlotItemID = SlotItemIDIntProp->GetPropertyValue_InContainer(SlotItemStructMemory);
            
            if (SlotItemID == ItemID) {
                Slot->Count -= 1;
                --RemainingCount;
                
                if (Slot->Count == 0) {
                    InventorySlots.RemoveAt(Index);
                }
                break;
            }
        }
    }
    
    USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
    Change->InventoryName = InventoryName;
    Change->Type = ESimpleInventoryChangeType::MULTI_REMOVAL;
    
    OnInventoryChangeEvent.Broadcast(Change);
    Result = RemainingCount == 0;
}

/**
 * Clears all items from the inventory.
 * Broadcasts a change event of type CLEAR.
 */
void USimpleInventory::Clear() {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::Clear"));
    
    USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
    Change->InventoryName = InventoryName;
    Change->Type = ESimpleInventoryChangeType::CLEAR;
    
    InventorySlots.Empty();
    OnInventoryChangeEvent.Broadcast(Change);
}

/**
 * Gets the current number of item slots in the inventory.
 *
 * @param Result  Output parameter returning the number of occupied slots.
 */
void USimpleInventory::GetLength(int32& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::GetLength"));
    
    Result = InventorySlots.Num();
}

/**
 * Gets the maximum number of slots this inventory can hold.
 *
 * @param Result  Output parameter returning the maximum slot size.
 */
void USimpleInventory::GetMaxSize(int32& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::GetMaxSize"));
    
    Result = MaxSlotSize;
}

/**
 * Retrieves the inventory slot at a given index.
 *
 * @param Index   The index of the slot to retrieve.
 * @param Result  The inventory slot at the given index, or nullptr if the index is invalid.
 */
void USimpleInventory::GetSlot(const int32 Index,
                               USimpleInventorySlot*& Result) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::GetSlot || Index: %i"), Index);
    
    if (InventorySlots.IsValidIndex(Index)) {
        Result = InventorySlots[Index];
    }
    else {
        Result = nullptr;
    }
}

/**
 * Retrieves a copy of all current inventory slots.
 *
 * @param Result  Output array containing all current inventory slots.
 */
void USimpleInventory::GetSlots(TArray<USimpleInventorySlot*>& Result) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::GetSlots"));
    
    Result = InventorySlots;
}

/**
 * Checks if the inventory contains a specific item with an exact count.
 * Only returns true for an exact match, not greater-than or less-than.
 *
 * @param ItemID  The ID of the item to check.
 * @param Count   The exact count to check for.
 * @param Result  True if the inventory contains the item with the specified count, false otherwise.
 */
void USimpleInventory::HasItem(const int32 ItemID,
                               const int32 Count,
                               bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::HasItem || ItemID: %i | Count: %i"), ItemID, Count);
    
    const FName IDPropName = FName("ID");
    Result = false;
    
    for (USimpleInventorySlot* Slot : InventorySlots) {
        const UStruct* SlotItemStructType = Slot->Item.GetScriptStruct();
        void* SlotItemStructMemory = Slot->Item.GetMutableMemory();
        
        FProperty* SlotItemIDProp = SlotItemStructType->FindPropertyByName(IDPropName);
        if (!SlotItemIDProp) {
            UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventory::HasItem || ID does not exist on specified struct type"));
            return;
        }
        
        FIntProperty* SlotItemIDIntProp = CastField<FIntProperty>(SlotItemIDProp);
        int32 SlotItemID = SlotItemIDIntProp->GetPropertyValue_InContainer(SlotItemStructMemory);
        
        if (SlotItemID == ItemID) {
            Result = Slot->Count == Count;
            break;
        }
    }
}

/**
 * Copies the inventory data from another inventory instance.
 *
 * @param OtherInventory  The inventory to copy from.
 */
void USimpleInventory::CopyInventory(const USimpleInventory* OtherInventory) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::CopyInventory"));
    
    MaxSlotSize = OtherInventory->MaxSlotSize;
    InventorySlots = OtherInventory->InventorySlots;
    
    USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
    Change->InventoryName = InventoryName;
    Change->Type = ESimpleInventoryChangeType::COPY;
    OnInventoryChangeEvent.Broadcast(Change);
}

/**
 * Forces a broadcast of the inventory change event, useful for syncing or refreshing UIs
 * when no actual item change has occurred.
 */
void USimpleInventory::ForceOnChange() const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::ForceOnChange"));
    
    USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
    Change->InventoryName = InventoryName;
    Change->Type = ESimpleInventoryChangeType::FORCE;
    OnInventoryChangeEvent.Broadcast(Change);
}

/**
 * Forces the inventory array to resize to the maximum slot size.
 * This ensures the internal inventory slot array matches the configured capacity.
 * After resizing, it broadcasts an inventory change event of type FORCE
 * to notify listeners that the inventory structure has been forcibly updated.
 */
void USimpleInventory::ForceResize() {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::ForceResize"));
    
    InventorySlots.SetNum(MaxSlotSize);
    
    USimpleInventoryChange* Change = NewObject<USimpleInventoryChange>();
    Change->InventoryName = InventoryName;
    Change->Type = ESimpleInventoryChangeType::FORCE;
    OnInventoryChangeEvent.Broadcast(Change);
}

// Protected Functions



// Private Functions

void USimpleInventory::AddItemToNewSlot(FInstancedStruct Item,
                                        const int32 Count) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventory::AddItemToNewSlot || Creating new USimpleInventorySlot"));
    
    USimpleInventorySlot *slot = NewObject<USimpleInventorySlot>();
    slot->Item = Item;
    slot->Count = Count;
    InventorySlots.Emplace(slot);
}
