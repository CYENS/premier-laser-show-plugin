

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathFunctionsLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PREMIERELASERSHOW_API UMathFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    /**
     * @brief Linearly interpolates between two rotators.
     * @param A The start rotator.
     * @param B The end rotator.
     * @param Alpha The interpolation factor. 0.0 returns A, 1.0 returns B.
     * @return The interpolated rotator.
     */
    UFUNCTION(BlueprintPure, Category = "Math Functions Library")
    static FRotator LerpRotators(const FRotator& A, const FRotator& B, float Alpha);
    
    UFUNCTION(BlueprintPure, Category = "Math Functions Library")
    static FQuat LerpQuaternions(const FQuat& A, const FQuat& B, float Alpha);

    UFUNCTION(BlueprintPure, Category = "Math Functions Library")
    static FVector LerpVectors(const FVector& A, const FVector& B, float Alpha);
};
