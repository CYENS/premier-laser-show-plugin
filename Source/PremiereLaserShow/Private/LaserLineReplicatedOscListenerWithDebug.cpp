#include "LaserLineReplicatedOscListenerWithDebug.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "LaserLineReplicatedOscListener.h"
#include "LaserLineOscListenerComponent.h"

ALaserLineReplicatedOscListenerWithDebug::ALaserLineReplicatedOscListenerWithDebug(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer),
	ZHeight(50.0f),
	Thickness(15.0f),
	PersistSeconds(0.0f),
	bUseActorRotation(true),
	bDrawCenters(true),
	LineBaseColor(FColor::Red)
{
	bShowScreenDebugMessages = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ALaserLineReplicatedOscListenerWithDebug::BeginPlay()
{
	Super::BeginPlay();
	DrawInitialSnapshot();
}

void ALaserLineReplicatedOscListenerWithDebug::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALaserLineReplicatedOscListenerWithDebug::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bShowScreenDebugMessages)
	{
		GEngine->AddOnScreenDebugMessage(702, 5.0f, FColor::Yellow, TEXT("ALaserLineReplicatedOscListenerWithDebug::Tick"));
		GEngine->AddOnScreenDebugMessage(703, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *LaserLinesArraySerializer.ToString()));
	}
	DrawLines(LaserLinesArraySerializer.LaserLines);
}

#if WITH_EDITOR
void ALaserLineReplicatedOscListenerWithDebug::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (
		const FName Name = PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
		Name == GET_MEMBER_NAME_CHECKED(ALaserLineReplicatedOscListenerWithDebug, LaserOnlineOscListenerComponent)
	)
	{
		DrawInitialSnapshot();
	}
}
#endif

void ALaserLineReplicatedOscListenerWithDebug::DrawInitialSnapshot() const
{
	if (!LaserOnlineOscListenerComponent) return;
	DrawLines(LaserOnlineOscListenerComponent->LinesView);
}

void ALaserLineReplicatedOscListenerWithDebug::DrawLines(const TArray<FLaserLineStruct>& Lines) const
{
	for (int32 i = 0; i < Lines.Num(); ++i)
	{
		const FLaserLineStruct& L = Lines[i];
		DrawOneLineLocal(FVector2D(L.GetX(), L.GetY()), L.GetLength(), L.GetRotation());
	}
}

void ALaserLineReplicatedOscListenerWithDebug::DrawOneLineLocal(const FVector2D& CenterXY, float Length, float RotDeg) const
{
	const float Half = 0.5f * Length;
	const float Rad  = FMath::DegreesToRadians(RotDeg);
	const FVector2D Dir(FMath::Cos(Rad), FMath::Sin(Rad));

	const FVector LocalStart (CenterXY.X - Half * Dir.X, CenterXY.Y - Half * Dir.Y, ZHeight);
	const FVector LocalEnd   (CenterXY.X + Half * Dir.X, CenterXY.Y + Half * Dir.Y, ZHeight);
	const FVector LocalCenter(CenterXY.X,                CenterXY.Y,                ZHeight);

	const FTransform& TM = GetActorTransform();
	const FVector WorldStart  = bUseActorRotation ? TM.TransformPosition(LocalStart ) : (GetActorLocation() + LocalStart );
	const FVector WorldEnd    = bUseActorRotation ? TM.TransformPosition(LocalEnd   ) : (GetActorLocation() + LocalEnd   );
	const FVector WorldCenter = bUseActorRotation ? TM.TransformPosition(LocalCenter) : (GetActorLocation() + LocalCenter);

	const uint8 Var = static_cast<uint8>(FMath::Abs(FMath::RoundToInt(WorldCenter.X)) % 255);
	const FColor LineColor(LineBaseColor.R ^ Var, LineBaseColor.G, LineBaseColor.B, 255);

	const UWorld* World = GetWorld();
	if (!World) return;

	const bool bPersistent = PersistSeconds > 0.f; 
	constexpr uint8 DepthPriority = 0; 
	DrawDebugLine(World, WorldStart, WorldEnd, LineColor, bPersistent,PersistSeconds, DepthPriority, Thickness);

	if (bDrawCenters)
	{
		constexpr float PointSize = 6.f;
		DrawDebugPoint(World, WorldCenter, PointSize, LineColor, bPersistent, PersistSeconds);
	}
}
