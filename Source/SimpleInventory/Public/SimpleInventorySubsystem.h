// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SimpleInventoryStorage.h"
#include "SimpleInventorySlotStorage.h"
#include "SimpleInventorySubsystemStorage.h"

#include "SimpleInventorySubsystem.generated.h"

class USimpleInventory;
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
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetAllInventories(TMap<FName, USimpleInventory*>& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetInventory(const FName InventoryName,
                      USimpleInventory*& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetLength(const FName InventoryName,
                   int32& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetMaxSize(const FName InventoryName,
                    int32& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetSlot(const FName InventoryName,
                 const int32 Index,
                 USimpleInventorySlot*& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetSlots(const FName InventoryName,
                  TArray<USimpleInventorySlot*>& Result);
    
    UFUNCTION(BlueprintPure, Category="Simple Inventory Subsystem")
    void GetStorage(FSimpleInventorySubsystemStorage& Result) const;
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void HasItem(const FName InventoryName,
                 const int32 ItemID,
                 const int32 Count,
                 bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void RegisterInventory(const FName InventoryName,
                           const int32 MaxSlots,
                           USimpleInventory*& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void AddItem(const FName InventoryName,
                 FInstancedStruct Item,
                 const int32 Count,
                 bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void RemoveItemAtIndex(const FName InventoryName,
                           const int32 Index,
                           const int32 Count,
                           bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void RemoveItems(const FName InventoryName,
                     const TArray<FInstancedStruct>& Items,
                     bool& Result);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void Clear(const FName InventoryName);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void ClearAll();
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void CopyInventory(const FName InventoryName,
                       const USimpleInventory* OtherInventory);
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void ForceOnChange(const FName InventoryName) const;
    
    UFUNCTION(BlueprintCallable, Category="Simple Inventory Subsystem")
    void InflateFromStorage(const FSimpleInventorySubsystemStorage Storage);
    
private:
    UFUNCTION()
    void Find(const FName InventoryName,
              USimpleInventory*& Result) const;
    
    UFUNCTION()
    void HandleOnChangeEvent(USimpleInventoryChange* InventoryChange);
};
