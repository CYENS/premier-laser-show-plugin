#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserLineStruct.h"
#include "LaserLineOscListenerComponent.generated.h"

struct FOSCAddress;
struct FOSCMessage;
class UOSCServer;

UCLASS(Blueprintable, Category=LaserLineOsc, meta=(BlueprintSpawnableComponent))
class ULaserLineOscListenerComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="LaserLine", meta=(AllowPrivateAccess="true"))
	int32 NumLines;
	
	FLaserLineArray LaserLineArray;
	
public:
	ULaserLineOscListenerComponent();
	
	UPROPERTY(BlueprintReadOnly, Category="LaserLine")
	TArray<FLaserLineStruct> LinesView;
	
	UPROPERTY(BlueprintAssignable, Category="LaserLine")
	FLinesUpdated OnLinesUpdated;

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category="OSC", meta=(AllowPrivateAccess="true"))
	FString ListenAddress;

	UPROPERTY(EditAnywhere, Category="OSC", meta=(AllowPrivateAccess="true"))
	int32 ListenPort;

	UPROPERTY(EditAnywhere, Category="OSC", meta=(AllowPrivateAccess="true"))
	bool bOscOnListenServerOnly;

	UPROPERTY(EditAnywhere, Category="OSC|Debug", meta=(AllowPrivateAccess="true"))
	bool bShowScreenDebugMessages;

	UPROPERTY()
	UOSCServer* OscServer = nullptr;

	UFUNCTION()
	void OnOscLaserLine(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);
};
