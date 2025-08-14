// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaserLineStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LaserLineUtilsLibrary.generated.h"

UCLASS(meta=(BlueprintThreadSafe, ScriptName = "MathLibrary"))
class ULaserLineUtilsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="LaserLineStruct", meta=(NativeMakeFunc))
	static FLaserLineStruct MakeLaserLineStruct(const float X, const float Y, const float Length, const float Rotation) { return FLaserLineStruct::Make(X, Y, Length, Rotation); }

	UFUNCTION(BlueprintPure, Category="LaserLineStruct", meta=(NativeBreakFunc))
	static void BreakLaserLineStruct(const FLaserLineStruct& InVec, float& X, float& Y, float& Length, float& Rotation)
	{
		X = InVec.GetX();
		Y = InVec.GetY();
		Length = InVec.GetLength();
		Rotation = InVec.GetRotation();
	}
	
	UFUNCTION(BlueprintPure, Category="LaserLineStruct")
	static FString LaserLineToString(const FLaserLineStruct& InVec) { return InVec.ToString(); }

	UFUNCTION(BlueprintPure, Category="LaserLineStruct")
	static FString LaserLineArrayToString(const TArray<FLaserLineStruct>& InArray)
	{
		return FLaserLineArray::ToString(InArray);
	}
};
