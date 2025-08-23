// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"

#include "SimpleInventoryDefinitions.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct SIMPLEINVENTORY_API FSimpleInventoryDefinition
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Simple Inventory Definition")
    FName InventoryName;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Simple Inventory Definition")
    int32 MaxSlots;
};

UCLASS(ClassGroup=(SimpleInventory), Blueprintable, BlueprintType)
class SIMPLEINVENTORY_API USimpleInventoryDefinitions : public UDataAsset
{
    GENERATED_BODY()
    
public:
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Simple Inventory Definitions")
    TArray<FSimpleInventoryDefinition> Values;
};