// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimedEventManager.generated.h"

DECLARE_DYNAMIC_DELEGATE(FTimedEvent);
DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FConditionEvent);

enum class ETimedEventType : uint8
{
	Rotation,
	Location
};

USTRUCT()
struct FTargetLocationData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* OwnerActor;
	UPROPERTY()
	AActor* ActorToFollow;

	FVector StartLocation;
	FVector TargetLocation;
	float Duration;
	float Delay;
	float Timer;
	float OverridenLocationZ;
	bool bFollowMode;

	FConditionEvent ConditionDelegate;
	TArray<FTimedEvent> OnCompleteDelegate;
	FTimedEvent OnDelayedCompleteDelegate;
};

USTRUCT()
struct FTargetRotationData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* OwnerActor;

	FRotator StartRotation;
	FRotator TargetRotation;
	float Duration;
	float Timer;
};

USTRUCT()
struct FTimerEventData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* OwnerActor;
	FTimedEvent OnTimeEnds;
	float Duration;
	float Timer;
};

USTRUCT()
struct FProgressBarData
{
	GENERATED_BODY()

	UPROPERTY()
	class UProgressBar* ProgressBar;
	float StartValue;
	float EndValue;
	float Duration;
	float Timer;
};

USTRUCT()
struct FTextData
{
	GENERATED_BODY()

	UPROPERTY()
	class UTextBlock* TextBlock;
	FText FormatText;
	FString ArgumentKey;
	
	float StartValue;
	float EndValue;
	float Duration;
	float Timer;
};

USTRUCT()
struct FWidgetRenderOpacityData
{
	GENERATED_BODY()

	UPROPERTY()
	class UWidget* Widget;
	float StartValue;
	float EndValue;
	float Duration;
	float Timer;
	bool IsLooped = false;
};

USTRUCT()
struct FWidgetTransformData
{
	GENERATED_BODY()

	UPROPERTY()
	class UWidget* Widget;
	FVector2D StartValue;
	FVector2D OffsetValue;
	float Duration;
	float Timer;
	FVector2D FreeVariable1;

	FString Key;
	TArray<FWidgetTransformData> Queue;
	FTimedEvent OnCompleteDelegate;
};

USTRUCT()
struct FCanvasPanelSlotSizeData
{
	GENERATED_BODY()

	UPROPERTY()
	class UCanvasPanelSlot* Slot;
	FVector2D StartValue;
	FVector2D EndValue;
	float Duration;
	float Timer;
};

UCLASS()
class GUILDGAME_API ATimedEventManager : public AActor
{
	GENERATED_BODY()
	
	static void CreateTimedEvent(UWorld* World);
public:	
	// Sets default values for this actor's properties
	ATimedEventManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	
	static ATimedEventManager* ManagerInstance;
	TArray<FTargetRotationData> RotationData;
	TArray<FTargetLocationData> LocationData;
	TArray<FProgressBarData> ProgressBarData;
	TArray<FTextData> TextData;
	TArray<FWidgetRenderOpacityData> WidgetOpacityData;
	TArray<FWidgetTransformData> WidgetTransformData;
	TArray<FWidgetTransformData> WidgetAsyncTransformData;
	TArray<FCanvasPanelSlotSizeData> CanvasPanelSlotSizeData;
	
	TMap<FString, FTimerEventData> TimedEventMap;

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	static void Rotate(AActor* ActorToRotate, FRotator TargetRotation, float Duration, UWorld* World);
	static void Move(AActor* ActorToMove, FVector TargetLocation, float Duration, float Delay, FConditionEvent& ConditionDelegate, TArray<FTimedEvent>& OnComplete, FTimedEvent& OnDelayedCompleteDelegate, UWorld* World);
	static void MoveToActorAndFollow(AActor* ActorToMove, AActor* ActorToFollow, float Duration, float OverridenLocationZ, FConditionEvent& ConditionDelegate, UWorld* World);
	static void RemoveLocationTimer(AActor* ActorToMove);
	
	static void CallEventWithDelay(AActor* EventActor, FString Key, FTimedEvent& EventToCall, float Duration, UWorld* World);
	static bool RemoveEventData(FString Key, bool bCallEvent);

	static void LerpProgressBar(class UProgressBar* ProgressBar, float Duration, float StartValue, float EndValue, UWorld* World);
	static void LerpTextNumber(class UTextBlock* TextBlock, FText FormatText, FString ArgumentKey, float Duration, float StartValue, float EndValue,  UWorld* World);

	static void LerpWidgetOpacity(class UWidget* Widget, float Duration, float StartValue, float EndValue, bool IsLooped,  UWorld* World);
	static void RemoveWidgetOpacityTimer(class UWidget* Widget);

	static void MoveWidget(class UWidget* Widget, float Duration, FVector2D OffsetValue, FTimedEvent& OnComplete, UWorld* World, bool AddToQueue = false);
	static void MoveWidgetAsync(class UWidget* Widget, float Duration, FVector2D OffsetValue, UWorld* World, TArray<FWidgetTransformData>* Queue = nullptr, FString* Key = nullptr);
	//static void MergeLerpWidgetTransform(class UWidget* Widget, float Duration, FVector2D OffsetValue, UWorld* World);
	static void CloneWidgetMovement(class UWidget* From, class UWidget* To);
	static void CloneWidgetMovementAsync(class UWidget* From, class UWidget* To, FString Key);
	static void RemoveWidgetTransformTimer(class UWidget* Widget);
	static FWidgetTransformData* FindWidgetTransformData(class UWidget* Widget);
	static void FindWidgetTransformAsyncData(UWidget* Widget, FString Key, TArray<FWidgetTransformData*>& OutFoundWidgets);
	static void FindWidgetTransformAsyncData(UWidget* Widget, TArray<FWidgetTransformData*>& OutFoundWidgets);

	static void LerpCanvasPanelSlotSize(class UCanvasPanelSlot* Slot, float Duration, FVector2D StartValue, FVector2D EndValue, UWorld* World);
	static void RemoveCanvasPanelSlotSizeTimer(class UCanvasPanelSlot* Slot);
	
	bool UpdateTextNumber(FTextData& Data, float DeltaTime);
	bool UpdateProgressBar(FProgressBarData& Data, float DeltaTime);
	bool UpdateActorRotation(FTargetRotationData& Data, float DeltaTime);
	bool UpdateActorLocation(FTargetLocationData& Data, float DeltaTime);
	bool UpdateWidgetOpacity(FWidgetRenderOpacityData& Data, float DeltaTime);
	bool UpdateWidgetTranslation(FWidgetTransformData& Data, float DeltaTime);
	bool UpdateWidgetTranslationAsync(FWidgetTransformData& Data, float DeltaTime);
	bool UpdateCanvasPanelSlotSize(FCanvasPanelSlotSizeData& Data, float DeltaTime);
};
