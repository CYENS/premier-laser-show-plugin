#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OSCManager.h"
#include "OSCServer.h"
#include "OSCClient.h"
#include "OSCAddress.h"
#include "OSCMessage.h"
#include "ReembodiedMachineOscHandler.generated.h"

// Delegates for incoming OSC events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSceneActivate, int32, SceneIndex, int32, PhaseIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightIntensitiesReceived, const TArray<float>&, Intensities);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightPansReceived, const TArray<FVector2f>&, PanValues);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightTiltsReceived, const TArray<FVector2f>&, TiltValues);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightOffsetReceived, const TArray<FVector2f>&, Offsets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLaserLinesReceived, const TArray<FVector4f>&, LineData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLightJointAssociationReceived, const TArray<int>&, LightJointAssociation);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PREMIERELASERSHOW_API UReembodiedMachineOscHandler : public UActorComponent
{
    GENERATED_BODY()

public:
    UReembodiedMachineOscHandler();
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /** OSC connections **/
    UPROPERTY(BlueprintReadWrite, Category="Connections")
    UOSCServer* OscServer;

    UPROPERTY(BlueprintReadWrite, Category="Connections")
    UOSCClient* OscClient;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Connections")
    FString SendIPAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Connections")
    int32 SendPort;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Connections")
    FString ReceiveIPAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Connections")
    int32 ReceivePort;

    /** OSC event dispatchers **/
    UPROPERTY(BlueprintAssignable, Category="OSC Events")
    FOnSceneActivate OnSceneActivate;

    UPROPERTY(BlueprintAssignable, Category="OSC Events")
    FOnLightIntensitiesReceived OnLightIntensitiesReceived;

    UPROPERTY(BlueprintAssignable, Category="OSC Events")
    FOnLightPansReceived OnLightPansReceived;

    UPROPERTY(BlueprintAssignable, Category="OSC Events")
    FOnLightTiltsReceived OnLightTiltsReceived;

    UPROPERTY(BlueprintAssignable, Category="OSC Events")
    FOnLightOffsetReceived OnLightOffsetsReceived;

    UPROPERTY(BlueprintAssignable, Category="OSC Events")
    FOnLaserLinesReceived OnLaserLinesReceived;
    
    UPROPERTY(BlueprintAssignable, Category="OSC Events")
    FOnLightJointAssociationReceived OnLightJointAssociationReceived;

protected:
    void InitializeOsc();
    void ShutdownOsc();

    // Individual handlers matching FOSCDispatchMessageEventBP signature
    UFUNCTION()
    void HandleSceneActivate(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);

    UFUNCTION()
    void HandleLightIntensity(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);

    UFUNCTION()
    void HandleLightPan(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);

    UFUNCTION()
    void HandleLightTilts(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);

    UFUNCTION()
    void HandleLightOffsets(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);

    UFUNCTION()
    void HandleLaserLines(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);
    
    UFUNCTION()
    void HandleLightJointAssociate(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort);
};
