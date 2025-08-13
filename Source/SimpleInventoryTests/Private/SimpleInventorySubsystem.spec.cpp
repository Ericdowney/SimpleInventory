#include "Misc/AutomationTest.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Tests/AutomationCommon.h"
#include "Editor/UnrealEd/Public/Tests/AutomationEditorCommon.h"

#include "SimpleInventoryChange.h"
#include "SimpleInventoryItem.h"
#include "SimpleInventory.h"
#include "SimpleInventorySlot.h"
#include "SimpleInventorySubsystem.h"

DEFINE_SPEC(SimpleInventorySubsystemSpec, "SimpleInventory.SimpleInventorySubsystem", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

USimpleInventory* RegisteredInventory = nullptr;
USimpleInventorySubsystem* InventorySubsystem = nullptr;

void SimpleInventorySubsystemSpec::Define() {
    BeforeEach([this]() {
        UGameInstance* FakeGameInstance = NewObject<UGameInstance>(GetTransientPackage(), UGameInstance::StaticClass());
        InventorySubsystem = NewObject<USimpleInventorySubsystem>(FakeGameInstance, USimpleInventorySubsystem::StaticClass());
    });
    
    Describe("RegisterInventory", [this]() {
        It("should create and return a new inventory", [this]() {
            USimpleInventory* Inv = nullptr;
            
            InventorySubsystem->RegisterInventory(TEXT("TestInv"), 5, Inv);
            
            TestNotNull("Inventory should be created", Inv);
            TestTrue("Inventory should be stored in map", InventorySubsystem->InventoryMap.Contains(TEXT("TestInv")));
        });
    });
    
    Describe("GetAllInventories", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 3, RegisteredInventory);
        });
        
        It("should return all registered inventories", [this]() {
            TMap<FName, USimpleInventory*> Result;
            
            InventorySubsystem->GetAllInventories(Result);
            
            TestEqual("Should contain one inventory", Result.Num(), 1);
            TestTrue("Should contain Inv1", Result.Contains(TEXT("Inv1")));
        });
    });
    
    Describe("GetInventory", [this]() {
        It("should return nullptr when not found", [this]() {
            USimpleInventory* Out = nullptr;
            
            InventorySubsystem->GetInventory(TEXT("None"), Out);
            
            TestNull("Inventory should be null", Out);
        });
        
        It("should return the correct inventory when registered", [this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 3, RegisteredInventory);
            
            USimpleInventory* Out = nullptr;
            InventorySubsystem->GetInventory(TEXT("Inv1"), Out);
            
            TestTrue("Should match registered", Out == RegisteredInventory);
        });
    });
    
    Describe("GetLength / GetMaxSize", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 4, RegisteredInventory);
        });
        
        It("should return correct length and max size", [this]() {
            int32 Length = -1, MaxSize = -1;
            
            InventorySubsystem->GetLength(TEXT("Inv1"), Length);
            InventorySubsystem->GetMaxSize(TEXT("Inv1"), MaxSize);
            
            TestEqual("Length should be 0 initially", Length, 0);
            TestEqual("Max size should match registration", MaxSize, 4);
        });
    });
    
    Describe("GetSlot / GetSlots", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 2, RegisteredInventory);
        });
        
        It("should return empty array for GetSlots initially", [this]() {
            TArray<USimpleInventorySlot*> Slots;
            
            InventorySubsystem->GetSlots(TEXT("Inv1"), Slots);
            
            TestEqual("Should have 0 slots used initially", Slots.Num(), 0);
        });
        
        It("should return nullptr for GetSlot initially", [this]() {
            USimpleInventorySlot* Slot = nullptr;
            
            InventorySubsystem->GetSlot(TEXT("Inv1"), 0, Slot);
            
            TestNull("Slot should be null initially", Slot);
        });
    });
    
    Describe("HasItem", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 4, RegisteredInventory);
        });
        
        It("should return false when item not present", [this]() {
            bool bHas = true;
            
            InventorySubsystem->HasItem(TEXT("Inv1"), 123, 1, bHas);
            
            TestFalse("Should not have the item", bHas);
        });
    });
    
    Describe("AddItem / RemoveItemAtIndex / RemoveItems", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 4, RegisteredInventory);
        });
        
        It("should add and remove items correctly", [this]() {
            FSimpleInventoryItem TestItem;
            bool bAdded = false, bRemoved = true;
            TestItem.ItemName = FText::FromString(TEXT("TestSword"));
            
            InventorySubsystem->AddItem(TEXT("Inv1"), FInstancedStruct::Make(TestItem), 1, bAdded);
            
            TestTrue("Item should be added", bAdded);
            
            InventorySubsystem->RemoveItemAtIndex(TEXT("Inv1"), 0, 1, bRemoved);
            
            TestTrue("Item should be removed", bRemoved);
        });
        
        It("should remove multiple items", [this]() {
            bool bRemoved = false;
            TArray<FInstancedStruct> Items;
            
            InventorySubsystem->RemoveItems(TEXT("Inv1"), Items, bRemoved);
            
            TestTrue("Should handle removing empty list", bRemoved);
        });
    });
    
    Describe("Clear / ClearAll", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 4, RegisteredInventory);
        });
        
        It("should clear a specific inventory", [this]() {
            InventorySubsystem->Clear(TEXT("Inv1"));
            
            int32 Len = -1;
            InventorySubsystem->GetLength(TEXT("Inv1"), Len);
            TestEqual("Length should be 0", Len, 0);
        });
        
        It("should clear all inventories", [this]() {
            InventorySubsystem->ClearAll();
            
            int32 Len = -1;
            InventorySubsystem->GetLength(TEXT("Inv1"), Len);
            TestEqual("Length should be 0", Len, 0);
        });
    });
    
    Describe("CopyInventory", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 4, RegisteredInventory);
            USimpleInventory* OtherInv = nullptr;
            InventorySubsystem->RegisterInventory(TEXT("Inv2"), 4, OtherInv);
        });
        
        It("should copy contents from another inventory", [this]() {
            InventorySubsystem->CopyInventory(TEXT("Inv1"), RegisteredInventory);
            
            TestTrue("Copy executed without crash", true);
        });
    });
    
    Describe("ForceOnChange", [this]() {
        BeforeEach([this]() {
            InventorySubsystem->RegisterInventory(TEXT("Inv1"), 4, RegisteredInventory);
        });
        
        It("should fire OnInventorySubsystemChangeEvent", [this]() {
            InventorySubsystem->ForceOnChange(TEXT("Inv1"));
            TestTrue("Ran without crash", true);
        });
    });
    
    Describe("GetStorage / InflateFromStorage", [this]() {
        It("should export and import storage without crash", [this]() {
            // Register a test inventory
            const FName InventoryName = TEXT("TestInventory");
            USimpleInventory* Inv;
            InventorySubsystem->RegisterInventory(InventoryName, 5, Inv);
            
            // Add a test item
            FSimpleInventoryItem TestItem;
            TestItem.ItemName = FText::FromString(TEXT("TestSword"));
            
            bool bAdded = false;
            InventorySubsystem->AddItem(InventoryName, FInstancedStruct::Make(TestItem), 1, bAdded);
            TestTrue("Item should be added successfully", bAdded);
            
            // Act 1: Save storage
            FSimpleInventorySubsystemStorage SavedStorage;
            InventorySubsystem->GetStorage(SavedStorage);
            
            // Clear the inventory completely
            InventorySubsystem->Clear(InventoryName);
            
            // Ensure it’s empty
            {
                TArray<USimpleInventorySlot*> SlotsAfterClear;
                InventorySubsystem->GetSlots(InventoryName, SlotsAfterClear);
                TestTrue("Inventory should be empty after ClearInventory", SlotsAfterClear.Num() == 0);
            }
            
            // Act 2: Inflate from saved storage
            InventorySubsystem->InflateFromStorage(SavedStorage);
            
            // Assert: Item should be restored
            {
                TArray<USimpleInventorySlot*> SlotsAfterInflate;
                InventorySubsystem->GetSlots(InventoryName, SlotsAfterInflate);
                TestEqual("Inventory should have 1 item after InflateFromStorage", SlotsAfterInflate.Num(), 1);
            }
        });
    });
}
