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
    
    void AddItem(FInstancedStruct Item,
                 const int32 Count,
                 bool& Result);
    
    void RemoveItemAtIndex(const int32 Index,
                           const int32 Count,
                           bool& Result);
    
    void RemoveItems(const TArray<FInstancedStruct>& Items,
                     bool& Result);
    
    void Clear();
    
    void GetLength(int32& Result) const;
    
    void GetMaxSize(int32& Result) const;
    
    void GetSlot(const int32 Index,
                 USimpleInventorySlot*& Result) const;
    
    void GetSlots(TArray<USimpleInventorySlot*>& Result) const;
    
    void CopyInventory(const USimpleInventory* OtherInventory);
    
    void ForceOnChange() const;
    
protected:
    void BeginPlay() override;
};
