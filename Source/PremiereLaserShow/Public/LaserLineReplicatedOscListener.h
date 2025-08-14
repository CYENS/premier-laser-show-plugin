#pragma once

#include "CoreMinimal.h"
#include "LaserLineStruct.h"
#include "GameFramework/Actor.h"
#include "LaserLineReplicatedOscListener.generated.h"

class ULaserLineOscListenerComponent;
struct FOSCAddress;
struct FOSCMessage;
class UOSCServer;


UCLASS()
class ALaserLineReplicatedOscListener : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable, Category="Laser")
	FLinesUpdated OnLaserLinesUpdated;
	
protected:
	ALaserLineReplicatedOscListener(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing=OnRep_LaserLines)
	FLaserLineArray LaserLinesArraySerializer;

	UFUNCTION()
	void OnRep_LaserLines();
	
	UFUNCTION()
	void OnLaserLinesUpdated_Internal(const TArray<FLaserLineStruct>& Lines);
	
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LaserLines")
	TObjectPtr<ULaserLineOscListenerComponent> LaserOnlineOscListenerComponent;
	
	UPROPERTY(BlueprintReadOnly, Category="LaserLines")
	TArray<FLaserLineStruct> LaserLines;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LaserLines|Debug")
	bool bShowScreenDebugMessages;
};
