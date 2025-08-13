// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StructUtils/InstancedStruct.h"

#include "SimpleInventoryChangeType.h"

#include "SimpleInventoryChange.generated.h"

UCLASS(ClassGroup=(SimpleInventory), Blueprintable, BlueprintType)
class SIMPLEINVENTORY_API USimpleInventoryChange : public UObject
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Change")
    FName InventoryName;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Change")
    ESimpleInventoryChangeType Type = ESimpleInventoryChangeType::ADDITION;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Change")
    FInstancedStruct Item;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Change")
    int32 Count = 0;
};
