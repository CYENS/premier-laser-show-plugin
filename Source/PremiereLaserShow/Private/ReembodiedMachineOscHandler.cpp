#include "ReembodiedMachineOscHandler.h"
#include "OSCManager.h"
#include "OSCServer.h"
#include "OSCClient.h"

UReembodiedMachineOscHandler::UReembodiedMachineOscHandler()
{
    PrimaryComponentTick.bCanEverTick = false;
    SendIPAddress = TEXT("127.0.0.1");
    SendPort = 57121;
    ReceiveIPAddress = TEXT("0.0.0.0");
    ReceivePort = 57120;
}

void UReembodiedMachineOscHandler::BeginPlay()
{
    Super::BeginPlay();
    InitializeOsc();
}

void UReembodiedMachineOscHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ShutdownOsc();
    Super::EndPlay(EndPlayReason);
}

void UReembodiedMachineOscHandler::InitializeOsc()
{
    OscServer = UOSCManager::CreateOSCServer(ReceiveIPAddress, ReceivePort, true, true, TEXT("Reembodied_Server"), this);
    OscClient = UOSCManager::CreateOSCClient(SendIPAddress, SendPort, TEXT("Reembodied_Client"), this);

    // Bind each OSC pattern to its own event and handler
    const FOSCAddress ScenePattern = UOSCManager::ConvertStringToOSCAddress(TEXT("/scene/activate"));
    FOSCDispatchMessageEventBP SceneActivateEvent;
    SceneActivateEvent.BindDynamic(this, &UReembodiedMachineOscHandler::HandleSceneActivate);
    OscServer->BindEventToOnOSCAddressPatternMatchesPath(ScenePattern, SceneActivateEvent);

    const FOSCAddress IntensityPattern = UOSCManager::ConvertStringToOSCAddress(TEXT("/light/intensity"));
    FOSCDispatchMessageEventBP IntensityEvent;
    IntensityEvent.BindDynamic(this, &UReembodiedMachineOscHandler::HandleLightIntensity);
    OscServer->BindEventToOnOSCAddressPatternMatchesPath(IntensityPattern, IntensityEvent);

    const FOSCAddress PanPattern = UOSCManager::ConvertStringToOSCAddress(TEXT("/light/pan"));
    FOSCDispatchMessageEventBP PanEvent;
    PanEvent.BindDynamic(this, &UReembodiedMachineOscHandler::HandleLightPan);
    OscServer->BindEventToOnOSCAddressPatternMatchesPath(PanPattern, PanEvent);

    const FOSCAddress TiltPattern = UOSCManager::ConvertStringToOSCAddress(TEXT("/light/tilt"));
    FOSCDispatchMessageEventBP TiltEvent;
    TiltEvent.BindDynamic(this, &UReembodiedMachineOscHandler::HandleLightTilts);
    OscServer->BindEventToOnOSCAddressPatternMatchesPath(TiltPattern, TiltEvent);

    const FOSCAddress OffsetPattern = UOSCManager::ConvertStringToOSCAddress(TEXT("/light/offset"));
    FOSCDispatchMessageEventBP OffsetEvent;
    OffsetEvent.BindDynamic(this, &UReembodiedMachineOscHandler::HandleLightOffsets);
    OscServer->BindEventToOnOSCAddressPatternMatchesPath(OffsetPattern, OffsetEvent);

    const FOSCAddress LaserPattern = UOSCManager::ConvertStringToOSCAddress(TEXT("/laser/line"));
    FOSCDispatchMessageEventBP LaserEvent;
    LaserEvent.BindDynamic(this, &UReembodiedMachineOscHandler::HandleLaserLines);
    OscServer->BindEventToOnOSCAddressPatternMatchesPath(LaserPattern, LaserEvent);
    
    const FOSCAddress LightJointAssociatePattern = UOSCManager::ConvertStringToOSCAddress(TEXT("/light/joint/associate"));
    FOSCDispatchMessageEventBP LightJointAssociateEvent;
    LightJointAssociateEvent.BindDynamic(this, &UReembodiedMachineOscHandler::HandleLightJointAssociate);
    OscServer->BindEventToOnOSCAddressPatternMatchesPath(LightJointAssociatePattern, LightJointAssociateEvent);
}

void UReembodiedMachineOscHandler::ShutdownOsc()
{
    if (OscServer) { OscServer->Stop(); OscServer = nullptr; }
}

void UReembodiedMachineOscHandler::HandleSceneActivate(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort)
{
    TArray<int32> Ints;
    UOSCManager::GetAllInt32s(Message, Ints);
    if (Ints.Num() >= 2)
    {
        const int SceneIndex = Ints[0];
        const int PhaseIndex = Ints[1];
        OnSceneActivate.Broadcast(SceneIndex, PhaseIndex);
    }
}

void UReembodiedMachineOscHandler::HandleLightIntensity(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort)
{
    TArray<float> Intensities;
    UOSCManager::GetAllFloats(Message, Intensities);
    if (Intensities.Num() >= 16)
    {
        OnLightIntensitiesReceived.Broadcast(Intensities);
    }
}

void UReembodiedMachineOscHandler::HandleLightPan(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort)
{
    TArray<float> Values;
    TArray<FVector2f> Pans;
    Pans.Reserve(Values.Num() / 2);
    UOSCManager::GetAllFloats(Message, Values);
    for (int i = 0; i < Values.Num(); i+=2)
    {
        const float PanX = Values[i];
        const float PanY = Values[i + 1];
        const FVector2f Pan {PanX, PanY};
        Pans.Add(Pan);
    }
    OnLightPansReceived.Broadcast(Pans);
}

void UReembodiedMachineOscHandler::HandleLightTilts(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort)
{
    TArray<float> Values;
    TArray<FVector2f> Tilts;
    Tilts.Reserve(Values.Num() / 2);
    UOSCManager::GetAllFloats(Message, Values);
    for (int i = 0; i < Values.Num(); i+=2)
    {
        const float TiltX = Values[i];
        const float TiltY = Values[i + 1];
        const FVector2f Tilt {TiltX, TiltY};
        Tilts.Add(Tilt);
    }
    OnLightTiltsReceived.Broadcast(Tilts);
}

void UReembodiedMachineOscHandler::HandleLightOffsets(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort)
{
    
    TArray<float> Values;
    TArray<FVector2f> Offsets;
    Offsets.Reserve(Values.Num() / 2);
    UOSCManager::GetAllFloats(Message, Values);
    for (int i = 0; i < Values.Num(); i+=2)
    {
        const float OffsetX = Values[i];
        const float OffsetY = Values[i + 1];
        const FVector2f Offset {OffsetX, OffsetY};
        Offsets.Add(Offset);
    }
    OnLightOffsetsReceived.Broadcast(Offsets);
}

void UReembodiedMachineOscHandler::HandleLaserLines(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort)
{
    TArray<float> Values;
    TArray<FVector4f> LaserLines;
    LaserLines.Reserve(Values.Num() / 4);
    UOSCManager::GetAllFloats(Message, Values);
    for (int i = 0; i < Values.Num(); i+=4)
    {
        const float LineX = Values[i];
        const float LineY = Values[i + 1];
        const float LineAngle = Values[i + 2];
        const float LineLength = Values[i + 3];
        const FVector4f Line { LineX, LineY, LineAngle, LineLength };
        LaserLines.Add(Line);
    }
    OnLaserLinesReceived.Broadcast(LaserLines);
}

void UReembodiedMachineOscHandler::HandleLightJointAssociate(
    const FOSCAddress& AddressPattern,
    const FOSCMessage& Message,
    const FString& SenderIP,
    int32 SenderPort
)
{
    TArray<int> Values;
    UOSCManager::GetAllInt32s(Message, Values);
    OnLightJointAssociationReceived.Broadcast(Values);
}
