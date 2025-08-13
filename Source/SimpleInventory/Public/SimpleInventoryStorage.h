// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SimpleInventorySlotStorage.h"

#include "SimpleInventoryStorage.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct SIMPLEINVENTORY_API FSimpleInventoryStorage
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Storage")
    TArray<FSimpleInventorySlotStorage> StoredSlots;
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Simple Inventory Storage")
    int32 MaxSlots = 0;
};
