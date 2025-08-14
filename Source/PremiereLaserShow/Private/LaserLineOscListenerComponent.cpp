#include "LaserLineOscListenerComponent.h"

#include "OSCAddress.h"
#include "OSCManager.h"
#include "OSCServer.h"
#include "LogPremiereLaserShow.h"

ULaserLineOscListenerComponent::ULaserLineOscListenerComponent():
	NumLines(16),
	bShowScreenDebugMessages(false),
	ListenAddress(TEXT("0.0.0.0")),
	ListenPort(10000),
	bOscOnListenServerOnly(true)
{
}

void ULaserLineOscListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	const ENetMode NetMode = GetNetMode();
	const bool bShouldStartListeningToOsc = NetMode == NM_Standalone || NetMode == NM_DedicatedServer || GetOwner()->HasAuthority() && (!bOscOnListenServerOnly || NetMode == NM_ListenServer);
	if (!bShouldStartListeningToOsc)
	{
		return;
	}

    OscServer = UOSCManager::CreateOSCServer(ListenAddress, ListenPort, true, true, TEXT("Reembodied_Server"), this);
	if (!ensure(OscServer))
	{
		UE_LOG(LogPremiereLaserShow, Error, TEXT("Failed to create OSC server."));
		if (bShowScreenDebugMessages && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Failed to create OSC server."));
		}
		return;
	}
	UE_LOG(LogPremiereLaserShow, Log, TEXT("Created OSC server on %s:%d"), *ListenAddress, ListenPort);
	if (bShowScreenDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Created OSC server on %s:%d"), *ListenAddress, ListenPort));
	}
	LaserLineArray.LaserLines.SetNumUninitialized(NumLines);
	
	FOSCDispatchMessageEventBP Dispatch;
    Dispatch.BindDynamic(this, &ThisClass::OnOscLaserLine);
	OscServer->BindEventToOnOSCAddressPatternMatchesPath(FOSCAddress(TEXT("/laser/line")), Dispatch);
}

void ULaserLineOscListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OscServer && OscServer->IsActive())
	{
		// No explicit Stop() required; letting the UObject die is fine.
	}
	OscServer = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ULaserLineOscListenerComponent::OnOscLaserLine(const FOSCAddress& AddressPattern, const FOSCMessage& Message, const FString& SenderIP, int32 SenderPort)
{
	TArray<float> Values;
	constexpr int32 ExpectedNumValues = 64;
	UOSCManager::GetAllFloats(Message, Values);
	if (Values.Num() != ExpectedNumValues)
	{
		UE_LOG(LogTemp, Warning, TEXT("OSC /laser/line expected 16 args, got %d"), Values.Num());
		return;
	}

	for (int32 i = 0; i < NumLines; ++i)
	{
		const int32 Base = i * 4;
		const float X       = Values[Base + 0];
		const float Y       = Values[Base + 1];
		const float Length  = Values[Base + 2];
		const float Rotation= Values[Base + 3];

		FLaserLineStruct& Item = LaserLineArray.LaserLines[i];
		Item = FLaserLineStruct::Make(X, Y, Length, Rotation);
	}
	
	LinesView = LaserLineArray.LaserLines;
	OnLinesUpdated.Broadcast(LinesView);
	if (bShowScreenDebugMessages && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(100, 5.0f, FColor::Green, TEXT("ULaserLineOscListenerComponent::OnOscLaserLine"));
		GEngine->AddOnScreenDebugMessage(101, 5.0f, FColor::Green, *LaserLineArray.ToString());
	}
}