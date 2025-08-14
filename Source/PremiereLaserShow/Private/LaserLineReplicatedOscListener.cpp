
#include "LaserLineReplicatedOscListener.h"

#include "LaserLineOscListenerComponent.h"
#include "Net/UnrealNetwork.h"

ALaserLineReplicatedOscListener::ALaserLineReplicatedOscListener(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer),
	LaserOnlineOscListenerComponent(CreateDefaultSubobject<ULaserLineOscListenerComponent>(TEXT("LaserLineOscListenerComponent")))
{
	bReplicates = true;
	bAlwaysRelevant = true;
	AActor::SetReplicateMovement(false);
}

void ALaserLineReplicatedOscListener::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ALaserLineReplicatedOscListener, LaserLinesArraySerializer, COND_SkipOwner);
}

void ALaserLineReplicatedOscListener::BeginPlay()
{
	Super::BeginPlay();
	LaserOnlineOscListenerComponent->OnLinesUpdated.AddDynamic(this, &ALaserLineReplicatedOscListener::OnLaserLinesUpdated_Internal);
}

void ALaserLineReplicatedOscListener::OnLaserLinesUpdated_Internal(const TArray<FLaserLineStruct>& Lines)
{
	LaserLines = Lines;
	LaserLinesArraySerializer.LaserLines = Lines;
	LaserLinesArraySerializer.MarkArrayDirty();
	OnLaserLinesUpdated.Broadcast(Lines);
	if (bShowScreenDebugMessages)
	{
		GEngine->AddOnScreenDebugMessage(700, 5.0f, FColor::Purple, TEXT("ALaserLineOscReplicator::OnLaserLinesUpdated_Internal"));
		GEngine->AddOnScreenDebugMessage(701, 5.0f, FColor::Purple, FString::Printf(TEXT("%s"), *LaserLinesArraySerializer.ToString()));
	}
}

void ALaserLineReplicatedOscListener::OnRep_LaserLines()
{
	LaserLines = LaserLinesArraySerializer.LaserLines;
	OnLaserLinesUpdated_Internal(LaserLinesArraySerializer.LaserLines);
	if (bShowScreenDebugMessages)
	{
		GEngine->AddOnScreenDebugMessage(700, 5.0f, FColor::Yellow, TEXT("ALaserLineReplicatedOscListener::OnRep_LaserLines"));
		GEngine->AddOnScreenDebugMessage(701, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *LaserLinesArraySerializer.ToString()));
	}
}
