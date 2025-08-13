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
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void AddItem(FInstancedStruct Item,
                 const int32 Count,
                 bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void RemoveItemAtIndex(const int32 Index,
                           const int32 Count,
                           bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void RemoveItems(const TArray<FInstancedStruct>& Items,
                     bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void Clear();
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetLength(int32& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetMaxSize(int32& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetSlot(const int32 Index,
                 USimpleInventorySlot*& Result) const;
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void GetSlots(TArray<USimpleInventorySlot*>& Result) const;
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory")
    void HasItem(const int32 ItemID,
                 const int32 Count,
                 bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void CopyInventory(const USimpleInventory* OtherInventory);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void ForceOnChange() const;

    UFUNCTION(BlueprintCallable, Category="Simple Inventory")
    void ForceResize();
    
protected:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category="Simple Inventory")
    TArray<USimpleInventorySlot*> InventorySlots;
    
private:
    
    void AddItemToNewSlot(FInstancedStruct Item,
                          const int32 Count);
};
