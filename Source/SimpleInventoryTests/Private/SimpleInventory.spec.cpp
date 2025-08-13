#include "Misc/AutomationTest.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/Engine.h"

#include "SimpleInventoryItem.h"
#include "SimpleInventory.h"
#include "SimpleInventorySlot.h"

static FInstancedStruct MakeTestItem(int32 ID, bool bIsStackable = true, int32 StackSize = 10)
{
    FSimpleInventoryItem ItemMetadata;
    ItemMetadata.ID = ID;
    ItemMetadata.bIsStackable = bIsStackable;
    ItemMetadata.StackSize = StackSize;
    return FInstancedStruct::Make(ItemMetadata);
}

DEFINE_SPEC(SimpleInventorySpec, "SimpleInventory.Inventory", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

USimpleInventory* TestInventory = nullptr;

void SimpleInventorySpec::Define() {
    BeforeEach([this]() {
        TestInventory = NewObject<USimpleInventory>();
        TestInventory->InventoryName = TEXT("TestInv");
        TestInventory->MaxSlotSize = 5;
    });

    Describe("AddItem", [this]() {
        It("should add an item to a new slot", [this]() {
            bool bResult = false;
            FInstancedStruct Item = MakeTestItem(1);
            TestInventory->AddItem(Item, 1, bResult);
            TestTrue("Item added successfully", bResult);
            
            int32 Len;
            TestInventory->GetLength(Len);
            TestEqual("Inventory length should be 1", Len, 1);
        });

        It("should stack items in an existing slot", [this]() {
            bool bResult = false;
            FInstancedStruct Item = MakeTestItem(1, true, 5);
            TestInventory->AddItem(Item, 2, bResult);
            TestInventory->AddItem(Item, 2, bResult);
            TestTrue("Stacked successfully", bResult);

            USimpleInventorySlot* Slot = nullptr;
            TestInventory->GetSlot(0, Slot);

            TestEqual("Stack count should be 4", Slot->Count, 4);
        });

        It("should stack up to StackSize then create new slot", [this]() {
            bool bResult = false;
            TestInventory->AddItem(MakeTestItem(1, true, 3), 2, bResult);
            TestInventory->AddItem(MakeTestItem(1, true, 3), 3, bResult); // 1 in new slot
            TestTrue("Add successful", bResult);

            TArray<USimpleInventorySlot*> Slots;
            TestInventory->GetSlots(Slots);

            TestEqual("Should have 2 slots", Slots.Num(), 2);
            TestEqual("First slot count", Slots[0]->Count, 3);
            TestEqual("Second slot count", Slots[1]->Count, 2);
        });

        It("should fail gracefully when inventory is full", [this]() {
            bool bResult = false;
            // Fill inventory with non-stackable items
            for (int i = 0; i < TestInventory->MaxSlotSize; i++) {
                TestInventory->AddItem(MakeTestItem(i, false), 1, bResult);
            }
            TestInventory->AddItem(MakeTestItem(999, false), 1, bResult);

            TestFalse("AddItem should fail", bResult);
        });

        It("should fail gracefully when item is invalid", [this]() {
            bool bResult = false;
            FInstancedStruct InvalidItem;

            TestInventory->AddItem(InvalidItem, 1, bResult);

            int32 Len;
            TestInventory->GetLength(Len);
            TestFalse("AddItem should fail", bResult);
            TestEqual("Inventory length should be 0", Len, 0);
        });
    });

    Describe("RemoveItemAtIndex", [this]() {
        It("should remove items and delete slot when count reaches zero", [this]() {
            bool bResult = false;
            FInstancedStruct Item = MakeTestItem(1);
            TestInventory->AddItem(Item, 1, bResult);
            TestInventory->RemoveItemAtIndex(0, 1, bResult);
            TestTrue("Removal should succeed", bResult);
            int32 Len;
            TestInventory->GetLength(Len);
            TestEqual("Inventory should be empty", Len, 0);
        });

        It("should handle invalid index gracefully", [this]() {
            bool bResult = true;

            TestInventory->RemoveItemAtIndex(5, 1, bResult);

            TestFalse("Removal should fail", bResult);
        });

        It("should reduce count but keep slot if count > 0", [this]() {
            bool bResult = false;
            TestInventory->AddItem(MakeTestItem(1), 3, bResult);

            TestInventory->RemoveItemAtIndex(0, 1, bResult);

            USimpleInventorySlot* Slot;
            TestInventory->GetSlot(0, Slot);
            TestEqual("Remaining count", Slot->Count, 2);
        });
    });

    Describe("RemoveItems", [this]() {
        It("should remove matching items from inventory", [this]() {
            bool bResult = false;
            FInstancedStruct Item = MakeTestItem(1);
            TestInventory->AddItem(Item, 1, bResult);

            TArray<FInstancedStruct> ItemsToRemove = { MakeTestItem(1) };
            TestInventory->RemoveItems(ItemsToRemove, bResult);
            TestTrue("Removal should succeed", bResult);

            int32 Len;
            TestInventory->GetLength(Len);
            TestEqual("Inventory should be empty", Len, 0);
        });

        It("should fail if item not present", [this]() {
            bool bResult = true;
            TArray<FInstancedStruct> Items = { MakeTestItem(999) };

            TestInventory->RemoveItems(Items, bResult);

            TestFalse("Removal should fail", bResult);
        });
    });

    Describe("Clear", [this]() {
        It("should remove all slots", [this]() {
            bool bResult = false;
            TestInventory->AddItem(MakeTestItem(1), 1, bResult);

            TestInventory->Clear();

            int32 Len;
            TestInventory->GetLength(Len);
            TestEqual("Inventory should be empty", Len, 0);
        });
    });

    Describe("GetLength / GetMaxSize", [this]() {
        It("should return correct counts", [this]() {
            int32 Len;
            TestInventory->GetLength(Len);
            TestEqual("Initial length", Len, 0);

            int32 Max;
            TestInventory->GetMaxSize(Max);
            TestEqual("Max size should match", Max, 5);
        });
    });

    Describe("GetSlot / GetSlots", [this]() {
        It("should return slot references", [this]() {
            bool bResult = false;
            TestInventory->AddItem(MakeTestItem(1), 1, bResult);
            USimpleInventorySlot* Slot = nullptr;
            TestInventory->GetSlot(0, Slot);
            TestNotNull("Slot should not be null", Slot);

            TArray<USimpleInventorySlot*> Slots;
            TestInventory->GetSlots(Slots);
            TestEqual("Slots array size", Slots.Num(), 1);
        });
    });

    Describe("HasItem", [this]() {
        It("should return true for exact match", [this]() {
            bool bResult = false;
            TestInventory->AddItem(MakeTestItem(1), 3, bResult);
            bool bHas = false;
            TestInventory->HasItem(1, 3, bHas);
            TestTrue("Should have the item", bHas);
        });
    });

    Describe("CopyInventory", [this]() {
        It("should copy slots from another inventory", [this]() {
            bool bResult = false;
            TestInventory->AddItem(MakeTestItem(1), 1, bResult);

            USimpleInventory* Other = NewObject<USimpleInventory>();
            Other->InventoryName = TEXT("Other");
            Other->MaxSlotSize = 5;
            Other->CopyInventory(TestInventory);

            int32 Len;
            Other->GetLength(Len);
            TestEqual("Copied inventory length", Len, 1);
        });
    });

    Describe("ForceOnChange / ForceResize", [this]() {
        It("should resize slot array and broadcast", [this]() {
            TestInventory->ForceResize();
            int32 Len;
            TestInventory->GetLength(Len);
            TestEqual("Length should equal MaxSlotSize after resize", Len, 5);
        });
    });
}
