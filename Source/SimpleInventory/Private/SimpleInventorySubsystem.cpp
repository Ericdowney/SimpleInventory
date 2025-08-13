// Copyright Eric Downey - 2025

#include "SimpleInventorySubsystem.h"

#include "SimpleInventory.h"
#include "SimpleInventorySlot.h"
#include "SimpleInventoryLog.h"

// Lifecycle

/**
 * Initializes the inventory subsystem when the game starts.
 *
 * @param Collection A collection of subsystem dependencies.
 */
void USimpleInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection) {
    Super::Initialize(Collection);
    
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::Initialize"));
}

// Public Functions
/**
 * Retrieves a map of all registered inventories keyed by name.
 *
 * @param Result The output pointer to the registered inventories
 */
void USimpleInventorySubsystem::GetAllInventories(TMap<FName, USimpleInventory*>& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetAllInventories"));

    TMap<FName, USimpleInventory*> AllInventories;
    for (const auto& Item : InventoryMap) {
        AllInventories.Add(Item.Key, Item.Value);
    }
    Result = AllInventories;
}

/**
 * Retrieves a pointer to the inventory object associated with the given name.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Result The output pointer to the found inventory, or nullptr if not found.
 */
void USimpleInventorySubsystem::GetInventory(const FName InventoryName,
                                             USimpleInventory*& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetInventory || Inventory: %s"), *InventoryName.ToString());
    
    Find(InventoryName, Result);
}

/**
 * Retrieves the current item count in the specified inventory.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Result The number of items currently in the inventory.
 */
void USimpleInventorySubsystem::GetLength(const FName InventoryName,
                                          int32& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetLength || Inventory: %s"), *InventoryName.ToString());
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->GetLength(Result);
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::GetLength || Invalid Inventory: %s"), *InventoryName.ToString());
        Result = 0;
    }
}

/**
 * Retrieves the maximum allowed number of slots for a given inventory.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Result The maximum number of slots.
 */
void USimpleInventorySubsystem::GetMaxSize(const FName InventoryName,
                                           int32& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetMaxSize || Inventory: %s"), *InventoryName.ToString());
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->GetMaxSize(Result);
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::GetMaxSize || Invalid Inventory: %s"), *InventoryName.ToString());
        Result = 0;
    }
}

/**
 * Gets the inventory slot at a specified index.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Index The index of the slot to retrieve.
 * @param Result The inventory slot at the given index.
 */
void USimpleInventorySubsystem::GetSlot(const FName InventoryName,
                                        const int32 Index,
                                        USimpleInventorySlot*& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetSlot || Inventory: %s Index: %i"), *InventoryName.ToString(), Index);
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->GetSlot(Index, Result);
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::GetSlot || Invalid Inventory: %s"), *InventoryName.ToString());
        Result = nullptr;
    }
}

/**
 * Retrieves all inventory slots associated with the specified inventory.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Result The list of inventory slots.
 */
void USimpleInventorySubsystem::GetSlots(const FName InventoryName,
                                         TArray<USimpleInventorySlot*>& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetSlots || Inventory: %s"), *InventoryName.ToString());
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->GetSlots(Result);
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::GetSlots || Invalid Inventory: %s"), *InventoryName.ToString());
        Result = TArray<USimpleInventorySlot*>();
    }
}

/**
 * Serializes the state of all inventories managed by the subsystem.
 *
 * @param Result The output structure containing all stored inventory data.
 */
void USimpleInventorySubsystem::GetStorage(FSimpleInventorySubsystemStorage& Result) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetStorage"));
    
    TMap<FName, FSimpleInventoryStorage> Storage;
    for (auto& Item : InventoryMap) {
        FSimpleInventoryStorage StoredInventory = FSimpleInventoryStorage();
        
        USimpleInventory* Inventory = Item.Value.Get();
        
        if (IsValid(Inventory)) {
            StoredInventory.MaxSlots = Inventory->MaxSlotSize;
            TArray<USimpleInventorySlot*> Slots;
            Inventory->GetSlots(Slots);
            
            for (USimpleInventorySlot* Slot : Slots) {
                if (IsValid(Slot)) {
                    FSimpleInventorySlotStorage StoredSlot = FSimpleInventorySlotStorage();
                    StoredSlot.Metadata = Slot->Item;
                    StoredSlot.Count = Slot->Count;
                    
                    StoredInventory.StoredSlots.Add(StoredSlot);
                }
                else {
                    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetStorage || Found invalid InventorySlot"));
                }
            }
            
            Storage.Add(Item.Key, StoredInventory);
        }
        else {
            UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::GetStorage || Found invalid Inventory"));
        }
    }
    
    FSimpleInventorySubsystemStorage SubsystemStorage;
    SubsystemStorage.Value = Storage;
    Result = SubsystemStorage;
}

/**
 * Checks if the specified inventory contains a given item and quantity.
 *
 * @param InventoryName The identifier for the inventory.
 * @param ItemID The unique ID of the item to check.
 * @param Count The number of items to check for.
 * @param Result True if the inventory contains at least that amount of the item.
 */
void USimpleInventorySubsystem::HasItem(const FName InventoryName,
                                        const int32 ItemID,
                                        const int32 Count,
                                        bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::HasItem || Inventory: %s | ItemID: %i | Count: %i"), *InventoryName.ToString(), ItemID, Count);
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->HasItem(ItemID, Count, Result);
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::HasItem || Invalid Inventory: %s"), *InventoryName.ToString());
        Result = false;
    }
}

/**
 * Registers a new inventory with the given name and maximum slot count.
 * If it already exists, returns the existing inventory.
 *
 * @param InventoryName The identifier for the inventory.
 * @param MaxSlots The maximum number of slots for this inventory.
 * @param Result The registered or existing inventory instance.
 */
void USimpleInventorySubsystem::RegisterInventory(const FName InventoryName,
                                                  const int32 MaxSlots,
                                                  USimpleInventory*& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::RegisterInventory || Inventory: %s | MaxSlots: %i"), *InventoryName.ToString(), MaxSlots);
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (!Inventory) {
        USimpleInventory* NewInventory = NewObject<USimpleInventory>(this);
        if (!NewInventory) {
            UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::RegisterInventory || Failed to create new USimpleInventory for %s"), *InventoryName.ToString());
            Result = nullptr;
            return;
        }
        
        NewInventory->InventoryName = InventoryName;
        NewInventory->MaxSlotSize = MaxSlots;
        InventoryMap.Add(InventoryName, NewInventory);
        Result = NewInventory;
        
        NewInventory->OnInventoryChangeEvent.AddDynamic(this, &USimpleInventorySubsystem::HandleOnChangeEvent);
    }
    else {
        Result = Inventory;
    }
}

/**
 * Adds an item to the specified inventory.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Item The item instance to add.
 * @param Count The number of times to add the item.
 * @param Result True if the item was added successfully.
 */
void USimpleInventorySubsystem::AddItem(const FName InventoryName,
                                        FInstancedStruct Item,
                                        const int32 Count,
                                        bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::AddItem || Inventory: %s | Count: %i"), *InventoryName.ToString(), Count);
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->AddItem(Item, Count, Result);
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::AddItem || Invalid Inventory: %s"), *InventoryName.ToString());
        Result = false;
    }
}

/**
 * Removes a quantity of an item at a specified index from the inventory.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Index The index of the item slot to remove from.
 * @param Count The quantity to remove.
 * @param Result True if removal was successful.
 */
void USimpleInventorySubsystem::RemoveItemAtIndex(const FName InventoryName,
                                                  const int32 Index,
                                                  const int32 Count,
                                                  bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::RemoveItemAtIndex || Inventory: %s | Index: %i | Count: %i"), *InventoryName.ToString(), Index, Count);
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    Inventory->RemoveItemAtIndex(Index, Count, Result);
}

/**
 * Removes a list of items from the specified inventory.
 *
 * @param InventoryName The identifier for the inventory.
 * @param Items The items to remove.
 * @param Result True if the items were successfully removed.
 */
void USimpleInventorySubsystem::RemoveItems(const FName InventoryName,
                                            const TArray<FInstancedStruct>& Items,
                                            bool& Result) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::RemoveItems || Inventory: %s"), *InventoryName.ToString());
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    Inventory->RemoveItems(Items, Result);
}

/**
 * Clears all items from a specified inventory.
 *
 * @param InventoryName The identifier for the inventory to clear.
 */
void USimpleInventorySubsystem::Clear(const FName InventoryName) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::Clear || Inventory: %s"), *InventoryName.ToString());
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->Clear();
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::Clear || Invalid Inventory: %s"), *InventoryName.ToString());
    }
}

/**
 * Clears all inventories managed by this subsystem.
 */
void USimpleInventorySubsystem::ClearAll() {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::ClearAll"));
    
    for (auto& Item : InventoryMap) {
        auto Inventory = Item.Value.Get();
        if (IsValid(Inventory)) {
            Inventory->Clear();
        }
    }
}

/**
 * Copies the contents of one inventory into another.
 *
 * @param InventoryName The identifier of the inventory to copy into.
 * @param OtherInventory The source inventory to copy from.
 */
void USimpleInventorySubsystem::CopyInventory(const FName InventoryName,
                                              const USimpleInventory* OtherInventory) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::CopyInventory || Inventory: %s"), *InventoryName.ToString());
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->CopyInventory(OtherInventory);
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::CopyInventory || Invalid Inventory: %s"), *InventoryName.ToString());
    }
}

/**
 * Triggers an inventory change event manually for the specified inventory.
 *
 * @param InventoryName The identifier for the inventory.
 */
void USimpleInventorySubsystem::ForceOnChange(const FName InventoryName) const {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::ForceOnChange || Inventory: %s"), *InventoryName.ToString());
    
    USimpleInventory* Inventory;
    Find(InventoryName, Inventory);
    if (IsValid(Inventory)) {
        Inventory->ForceOnChange();
    }
    else {
        UE_LOG(SimpleInventoryLog, Error, TEXT("USimpleInventorySubsystem::ForceOnChange || Invalid Inventory: %s"), *InventoryName.ToString());
    }
}

/**
 * Restores Inventories from a saved storage struct.
 * @param Storage - Struct containing saved Inventory data
 */
void USimpleInventorySubsystem::InflateFromStorage(const FSimpleInventorySubsystemStorage Storage) {
    UE_LOG(SimpleInventoryLog, Verbose, TEXT("USimpleInventorySubsystem::InflateFromStorage"));
    
    for (auto& Item : Storage.Value) {
        USimpleInventory* NewInventory;
        Find(Item.Key, NewInventory);
        
        if (!NewInventory) {
            RegisterInventory(Item.Key, Item.Value.MaxSlots, NewInventory);
        }
        NewInventory->Clear();
        
        NewInventory->MaxSlotSize = Item.Value.MaxSlots;
        for (FSimpleInventorySlotStorage StoredSlot : Item.Value.StoredSlots) {
            bool Result = false;
            NewInventory->AddItem(StoredSlot.Metadata, StoredSlot.Count, Result);
        }
    }
}

// Protected Functions

void USimpleInventorySubsystem::Find(const FName InventoryName,
                                     USimpleInventory*& Result) const {
    auto InventoryPtr = InventoryMap.Find(InventoryName);
    Result = (InventoryPtr && *InventoryPtr) ? *InventoryPtr : nullptr;
}

// Private Functions

void USimpleInventorySubsystem::HandleOnChangeEvent(USimpleInventoryChange* InventoryChange) {
    OnInventorySubsystemChangeEvent.Broadcast(InventoryChange);
}
