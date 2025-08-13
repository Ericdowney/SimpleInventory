// Copyright Eric Downey - 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SimpleInventoryChangeType.generated.h"

UENUM(BlueprintType)
enum class ESimpleInventoryChangeType : uint8
{
    ADDITION UMETA(DisplayName = "Addition"),
    REMOVAL UMETA(DisplayName = "Removal"),
    MULTI_REMOVAL UMETA(DisplayName = "Multi Removal"),
    CLEAR UMETA(DisplayName = "Clear"),
    COPY UMETA(DisplayName = "Copy"),
    FORCE UMETA(DisplayName = "Force"),
    FULL UMETA(DisplayName = "Full")
};
