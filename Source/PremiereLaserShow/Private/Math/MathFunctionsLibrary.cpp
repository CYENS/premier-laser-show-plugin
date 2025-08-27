#include "Math/MathFunctionsLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Rotator Lerp (RLerp)
FRotator UMathFunctionsLibrary::LerpRotators(const FRotator& A, const FRotator& B, float Alpha)
{
    // UKismetMathLibrary::RLerp is the function for Rotators.
    // The 'true' parameter ensures the shortest path is taken, preventing spinning more than 180 degrees.
    return UKismetMathLibrary::RLerp(A, B, Alpha, true);
}

// Quaternion Lerp (SLerp)
FQuat UMathFunctionsLibrary::LerpQuaternions(const FQuat& A, const FQuat& B, float Alpha)
{
    // UKismetMathLibrary::SLerp is the function for Quaternions.
    // Note that the standard FQuat::Slerp is also an option, but using Kismet is a common practice.
    return UKismetMathLibrary::Quat_Slerp(A, B, Alpha);
}

// Vector Lerp
FVector UMathFunctionsLibrary::LerpVectors(const FVector& A, const FVector& B, float Alpha)
{
    // UKismetMathLibrary::VLerp is the function for Vectors.
    return UKismetMathLibrary::VLerp(A, B, Alpha);
}