
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Net/Core/Serialization/QuantizedVectorSerialization.h"
#include "Math/Float16.h"

#include "LaserLineStruct.generated.h"


USTRUCT(
	BlueprintType,
	meta=(
		HasNativeMake ="/Script/PremiereLaserShow.LaserLineUtilsLibrary.MakeLaserLineStruct",
		HasNativeBreak="/Script/PremiereLaserShow.LaserLineUtilsLibrary.BreakLaserLineStruct"
	)
)
struct FLaserLineStruct : public FFastArraySerializerItem
{
	GENERATED_BODY()

	// 3 comps with built-in quantization (0.1 precision). Pick _100 or _1 to taste.
	UPROPERTY() FVector_NetQuantize10 XYLength;

	// W as 16-bit half-float carried as raw bits (so it replicates as exactly 16 bits).
	// We convert to/from float with FFloat16 helpers in accessors.
	UPROPERTY() uint16 RotationHalf = 0;
	
	UPROPERTY() uint8 OpacityByte = 255; // 0=transparent, 255=opaque
	
	static FLaserLineStruct Make(const float X, const float Y, const float Length, const float Rotation)
	{
		FLaserLineStruct L;
		L.XYLength = FVector(X, Y, Length);
		L.SetRotation(Rotation);
		return L;
	}

	static FLaserLineStruct Make(const float X, const float Y, const float Length, const float Rotation, const float Opacity)
	{
		FLaserLineStruct L = Make(X, Y, Length, Rotation);
		L.SetOpacity(Opacity);
		return L;
	}
	
	float GetX() const
	{
		return XYLength.X;
	}

	float GetY() const
	{
		return XYLength.Y;
	}

	float GetLength() const
	{
		return XYLength.Z;
	}
	
	float GetOpacity() const
	{
		return static_cast<float>(OpacityByte) / 255.0f;
	}
	
	void SetOpacity(const float Opacity)
	{
		const float Clamped = FMath::Clamp(Opacity, 0.0f, 1.0f);
		OpacityByte = static_cast<uint8>(FMath::RoundToInt(Clamped * 255.0f));
	}
	
	float GetRotation() const
	{
		FFloat16 H;
		H.Encoded = RotationHalf;
		return H;
	}
	
	void SetRotation(const float Rotation)
	{
		const FFloat16 H = Rotation;
		RotationHalf = H.Encoded;
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("LaserLineStruct(X: %.2f, Y: %.2f, Length: %.2f, Rotation: %.1f°, Opacity: %.2f)"), GetX(), GetY(), GetLength(), GetRotation(), GetOpacity());
	}
};

USTRUCT()
struct FLaserLineArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLaserLineStruct> LaserLines;

	static FLaserLineArray Make(const int NumLines)
	{
		FLaserLineArray L;
		L.LaserLines.Reserve(NumLines);
		for (int i = 0; i < NumLines; ++i)
		{
			L.LaserLines.Add(FLaserLineStruct());
		}
		return L;
	}
	// Fast-array delta serialization
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& Params)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize(LaserLines, Params, *this);
	}

	FString ToString() const
	{
		return ToString(LaserLines);
	}

	static FString ToString(const TArray<FLaserLineStruct>& Lines)
	{

		FString DebugString;
		DebugString.Reserve(128 + Lines.Num() * 64);
		DebugString += FString::Printf(TEXT("NumLines: %d\n"), Lines.Num());
		int i = 0;
		for (const FLaserLineStruct& Line : Lines)
		{
			DebugString += FString::Printf(TEXT("Line %d: %s\n"), i++, *Line.ToString());
		}
		return DebugString;
	}
};

template<>
struct TStructOpsTypeTraits<FLaserLineArray> : TStructOpsTypeTraitsBase2<FLaserLineArray>
{
	enum { WithNetDeltaSerializer = true };
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLinesUpdated, const TArray<FLaserLineStruct>&, Lines);
