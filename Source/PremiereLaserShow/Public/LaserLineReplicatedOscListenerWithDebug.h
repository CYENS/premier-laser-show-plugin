#pragma once

#include "CoreMinimal.h"
#include "LaserLineReplicatedOscListener.h"
#include "GameFramework/Actor.h"
#include "LaserLineStruct.h"
#include "LaserLineReplicatedOscListenerWithDebug.generated.h"

class ALaserLineReplicatedOscListener;

UCLASS()
class ALaserLineReplicatedOscListenerWithDebug : public ALaserLineReplicatedOscListener
{
	GENERATED_BODY()

public:
	ALaserLineReplicatedOscListenerWithDebug(const FObjectInitializer& ObjectInitializer);

	/** Height above this actor where lines are drawn. */
	UPROPERTY(EditAnywhere, Category="Laser|Debug")
	float ZHeight;

	/** Line thickness. */
	UPROPERTY(EditAnywhere, Category="Laser|Debug")
	float Thickness;

	/** If > 0 lines persist for N seconds. If 0, they last one frame. */
	UPROPERTY(EditAnywhere, Category="Laser|Debug")
	float PersistSeconds;

	/** Apply this actor's rotation to the local XY and rotation. */
	UPROPERTY(EditAnywhere, Category="Laser|Debug")
	bool bUseActorRotation;

	/** Draw a small dot at each line center. */
	UPROPERTY(EditAnywhere, Category="Laser|Debug")
	bool bDrawCenters;

	/** Base color tint. */
	UPROPERTY(EditAnywhere, Category="Laser|Debug")
	FColor LineBaseColor;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void DrawInitialSnapshot() const;

	/** Draw helpers. */
	void DrawLines(const TArray<FLaserLineStruct>& Lines) const;
	void DrawOneLineLocal(const FVector2D& CenterXY, const float Length, const float RotDeg) const;
	void DrawOneLineLocal(const FVector2D& CenterXY, float Length, float RotDeg, float Opacity) const;

};
