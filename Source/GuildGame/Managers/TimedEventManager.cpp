// Fill out your copyright notice in the Description page of Project Settings.


#include "TimedEventManager.h"

#include "WidgetManager.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "GuildGame/Battle/BattlePlayerController.h"
#include "GuildGame/Widgets/PopupTextWidget.h"
#include "Kismet/GameplayStatics.h"

ATimedEventManager* ATimedEventManager::Instance = nullptr;

// Sets default values
ATimedEventManager::ATimedEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ATimedEventManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATimedEventManager::BeginDestroy()
{
	Super::BeginDestroy();
	Instance = nullptr;
}

// Called every frame
void ATimedEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetTickGroup(ETickingGroup::TG_PostPhysics);

	for (int i = RotationData.Num() - 1; i >= 0; --i)
	{
		if(UpdateActorRotation(RotationData[i], DeltaTime) == false)
		{
			RotationData.RemoveAt(i);
		}
	}

	for (int i = LocationData.Num() - 1; i >= 0; --i)
	{
		if(UpdateActorLocation(LocationData[i], DeltaTime) == false)
		{
			FTargetLocationData DataToRemove = LocationData[i];
			LocationData.RemoveAt(i);

			if(DataToRemove.bFollowMode == false)
			{
				if(DataToRemove.OnDelayedCompleteDelegate.IsBound())
				{
					DataToRemove.OnDelayedCompleteDelegate.Execute();
				}
			}
		}
	}

	TArray<FString> KeysToRemove;
	for (const TPair<FString, FTimerEventData>& Pair : TimedEventMap)
    {
      if(TimedEventMap.Contains(Pair.Key))
      {
     	FTimerEventData& EventData = TimedEventMap[Pair.Key];
	    EventData.Timer += DeltaTime;

     	if(EventData.Timer >= EventData.Duration || EventData.OwnerActor == nullptr)
		{
			KeysToRemove.Add(Pair.Key);
		}
      }
    }

	for (int i = 0; i < KeysToRemove.Num(); ++i)
	{
		RemoveEventData(KeysToRemove[i], true);
	}


	for (int i = ProgressBarData.Num() - 1; i >= 0; --i)
	{
		if(UpdateProgressBar(ProgressBarData[i], DeltaTime) == false)
		{
			ProgressBarData.RemoveAt(i);
		}
	}

	for (int i = TextData.Num() - 1; i >= 0; --i)
	{
		if(UpdateTextNumber(TextData[i], DeltaTime) == false)
		{
			TextData.RemoveAt(i);
		}
	}
	for (int i = WidgetOpacityData.Num() - 1; i >= 0; --i)
	{
		if(UpdateWidgetOpacity(WidgetOpacityData[i], DeltaTime) == false)
		{
			WidgetOpacityData.RemoveAt(i);
		}
	}

	for (int i = WidgetTransformData.Num() - 1; i >= 0; --i)
	{
		if(UpdateWidgetTranslation(WidgetTransformData[i], DeltaTime) == false)
		{
			WidgetTransformData.RemoveAt(i);
		}
	}

	for (int i = WidgetAsyncTransformData.Num() - 1; i >= 0; --i)
	{
		if(UpdateWidgetTranslationAsync(WidgetAsyncTransformData[i], DeltaTime) == false)
		{
			WidgetAsyncTransformData.RemoveAt(i);
		}
	}

	for (int i = CanvasPanelSlotSizeData.Num() - 1; i >= 0; --i)
	{
		if(UpdateCanvasPanelSlotSize(CanvasPanelSlotSizeData[i], DeltaTime) == false)
		{
			CanvasPanelSlotSizeData.RemoveAt(i);
		}
	}

	for (int i = PopupTextData.Num() - 1; i >= 0; --i)
	{
		if(UpdatePopupText(PopupTextData[i], DeltaTime) == false)
		{
			if(PopupTextData[i].PopupWidget && PopupTextPool.IsInitialized())
			{
				PopupTextPool.Release(PopupTextData[i].PopupWidget);
				PopupTextData[i].PopupWidget->RemoveFromViewport();
			}
			PopupTextData.RemoveAt(i);
		}
	}
}

void ATimedEventManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(PopupTextPool.IsInitialized())
	{
		PopupTextPool.ReleaseAllSlateResources();
	}
}

void ATimedEventManager::CreateManagerInstance(UWorld* World)
{
	if(World == nullptr) return;
	
	if(Instance == nullptr)
	{
		Instance = World->SpawnActor<ATimedEventManager>();
	}
}

void ATimedEventManager::Rotate(AActor* ActorToRotate, FRotator TargetRotation, float Duration, UWorld* World)
{
	if(ActorToRotate == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;
	
	for (int i = 0; i < Instance->RotationData.Num(); ++i)
	{
		if(Instance->RotationData[i].OwnerActor)
		{
			if(Instance->RotationData[i].OwnerActor == ActorToRotate)
			{
				Instance->RotationData.RemoveAt(i);
				break;
			}
		}
	}

	FTargetRotationData NewData;
	NewData.OwnerActor = ActorToRotate;
	NewData.StartRotation = ActorToRotate->GetActorRotation();
	NewData.TargetRotation = TargetRotation;
	NewData.Duration = Duration;
	NewData.Timer = 0;
	
	Instance->RotationData.Add(NewData);
}

void ATimedEventManager::Move(AActor* ActorToMove, FVector TargetLocation, float Duration, float Delay, FConditionEvent& ConditionDelegate, TArray<FTimedEvent>& OnComplete, FTimedEvent& OnDelayedCompleteDelegate, UWorld* World)
{
	if(ActorToMove == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;
	
	RemoveLocationTimer(ActorToMove);

	FTargetLocationData NewData;
	NewData.bFollowMode = false;
	NewData.OwnerActor = ActorToMove;
	NewData.StartLocation = ActorToMove->GetActorLocation();
	NewData.TargetLocation = TargetLocation;
	if(ConditionDelegate.IsBound())
	{
		NewData.ConditionDelegate = ConditionDelegate;
	}

	NewData.OnCompleteDelegate = OnComplete;
	NewData.OnDelayedCompleteDelegate = OnDelayedCompleteDelegate;
	NewData.ActorToFollow = nullptr;
	NewData.Duration = Duration;
	NewData.Delay = Delay;
	NewData.Timer = 0;
	
	Instance->LocationData.Add(NewData);
}

void ATimedEventManager::MoveToActorAndFollow(AActor* ActorToMove, AActor* ActorToFollow, float Duration, float OverridenLocationZ, FConditionEvent& ConditionDelegate, UWorld* World)
{
	if(ActorToMove == nullptr || ActorToFollow == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;
	
	RemoveLocationTimer(ActorToMove);

	FTargetLocationData NewData;
	NewData.bFollowMode = true;
	NewData.OverridenLocationZ = OverridenLocationZ;
	NewData.OwnerActor = ActorToMove;
	NewData.ActorToFollow = ActorToFollow;
	NewData.StartLocation = ActorToMove->GetActorLocation();
	if(ConditionDelegate.IsBound())
	{
		NewData.ConditionDelegate = ConditionDelegate;
	}
	NewData.Duration = Duration;
	NewData.Timer = 0;
	
	Instance->LocationData.Add(NewData);
}

void ATimedEventManager::RemoveLocationTimer(AActor* ActorToMove)
{
	if(Instance == nullptr) return;
	
	for (int i = 0; i < Instance->LocationData.Num(); ++i)
	{
		if(Instance->LocationData[i].OwnerActor == ActorToMove)
		{
			FTargetLocationData DataToRemove = Instance->LocationData[i];
			Instance->LocationData.RemoveAt(i);

			if(DataToRemove.bFollowMode == false)
			{
				for (int j = 0; j < DataToRemove.OnCompleteDelegate.Num(); ++j)
				{
					if(DataToRemove.OnCompleteDelegate[j].IsBound())
					{
						DataToRemove.OnCompleteDelegate[j].Execute();
					}
				}
			}

			return;
		}
	}
}

void ATimedEventManager::CallEventWithDelay(AActor* EventActor, FString Key, FTimedEvent& EventToCall, float Duration, UWorld* World)
{
	if(EventActor == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;

	RemoveEventData(Key, true);
	FTimerEventData EventData;
	EventData.Duration = Duration;
	EventData.Timer = 0;
	EventData.OwnerActor = EventActor;
	if(EventToCall.IsBound())
	{
		EventData.OnTimeEnds = EventToCall;
	}
	Instance->TimedEventMap.Add(Key, EventData);
}

bool ATimedEventManager::RemoveEventData(FString Key, bool bCallEvent)
{
	if(Instance == nullptr) return false;

	if(Instance->TimedEventMap.Contains(Key))
	{
		FTimerEventData EventData;
		Instance->TimedEventMap.RemoveAndCopyValue(Key, EventData);

		if(bCallEvent && EventData.OwnerActor)
		{
			if(EventData.OnTimeEnds.IsBound())
			{
				EventData.OnTimeEnds.Execute();
			}
		}
		
		return  true;
	}
	return false;
}

void ATimedEventManager::LerpProgressBar(UProgressBar* ProgressBar, float Duration, float StartValue, float EndValue, UWorld* World)
{
	if(ProgressBar == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;
	
	for (int i = 0; i < Instance->ProgressBarData.Num(); ++i)
	{
		if(Instance->ProgressBarData[i].ProgressBar)
		{
			if(Instance->ProgressBarData[i].ProgressBar == ProgressBar)
			{
				Instance->ProgressBarData.RemoveAt(i);
				break;
			}
		}
	}

	FProgressBarData NewData;
	NewData.ProgressBar = ProgressBar;
	NewData.StartValue = StartValue;
	NewData.EndValue = EndValue;
	NewData.Duration = Duration;
	NewData.Timer = 0;
	
	Instance->ProgressBarData.Add(NewData);
}

void ATimedEventManager::LerpTextNumber(UTextBlock* TextBlock, FText FormatText, FString ArgumentKey, float Duration,
	float StartValue, float EndValue, UWorld* World)
{
	if(TextBlock == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;
	
	for (int i = 0; i < Instance->TextData.Num(); ++i)
	{
		if(Instance->TextData[i].TextBlock)
		{
			if(Instance->TextData[i].TextBlock == TextBlock)
			{
				Instance->TextData.RemoveAt(i);
				break;
			}
		}
	}

	FTextData NewData;
	NewData.TextBlock = TextBlock;
	NewData.FormatText = FormatText;
	NewData.ArgumentKey = ArgumentKey;
	NewData.StartValue = StartValue;
	NewData.EndValue = EndValue;
	NewData.Duration = Duration;
	NewData.Timer = 0;
	
	Instance->TextData.Add(NewData);
}

void ATimedEventManager::LerpWidgetOpacity(UWidget* Widget, float Duration, float StartValue, float EndValue,
	bool IsLooped, UWorld* World)
{
	if(Widget == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;
	
	RemoveWidgetOpacityTimer(Widget);

	FWidgetRenderOpacityData NewData;
	NewData.Widget = Widget;
	NewData.StartValue = StartValue;
	NewData.EndValue = EndValue;
	NewData.Duration = Duration;
	NewData.IsLooped = IsLooped;
	NewData.Timer = 0;
	
	Instance->WidgetOpacityData.Add(NewData);
}

void ATimedEventManager::RemoveWidgetOpacityTimer(UWidget* Widget)
{
	if(Instance == nullptr) return;

	for (int i = 0; i < Instance->WidgetOpacityData.Num(); ++i)
	{
		if(Instance->WidgetOpacityData[i].Widget == Widget)
		{
			Instance->WidgetOpacityData.RemoveAt(i);
			return;
		}
	}
}

void ATimedEventManager::MoveWidget(UWidget* Widget, float Duration,FVector2D OffsetValue,
	FTimedEvent& OnComplete, UWorld* World,  bool AddToQueue)
{
	if(Widget == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;

	FWidgetTransformData NewData;
	NewData.Widget = Widget;
	NewData.StartValue = Widget->RenderTransform.Translation;
	NewData.OffsetValue = OffsetValue;
	NewData.Duration = Duration;
	NewData.OnCompleteDelegate = OnComplete;
	NewData.Timer = 0;

	FWidgetTransformData* PreviouslyAddedData = nullptr;
	if(AddToQueue)
	{
		PreviouslyAddedData = FindWidgetTransformData(Widget);
	}
	
	if(PreviouslyAddedData == nullptr)
	{
		RemoveWidgetTransformTimer(Widget);
		Instance->WidgetTransformData.Add(NewData);
	}
	else
	{
		PreviouslyAddedData->Queue.Add(NewData);
	}
}

void ATimedEventManager::MoveWidgetAsync(UWidget* Widget, float Duration, FVector2D OffsetValue, UWorld* World, TArray<FWidgetTransformData>* Queue, FString* Key)
{
	if(Widget == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;

	FWidgetTransformData NewData;
	NewData.Widget = Widget;
	// NewData.StartValue = Widget->RenderTransform.Translation;
	NewData.OffsetValue = OffsetValue;
	NewData.Duration = Duration;
	NewData.Timer = 0;
	NewData.FreeVariable1 = FVector2D::ZeroVector;

	if(Key)
	{
		NewData.Key = *Key;
	}
	if(Queue)
	{
		for (int i = 0; i < Queue->Num(); ++i)
		{
			(*Queue)[i].Widget = Widget;
			(*Queue)[i].Timer = 0;
			(*Queue)[i].FreeVariable1 = FVector2D::ZeroVector;
		}
		NewData.Queue = *Queue;
	}

	// RemoveWidgetTransformTimer(Widget);
	Instance->WidgetAsyncTransformData.Add(NewData);
}

// void ATimedEventManager::MergeLerpWidgetTransform(UWidget* Widget, float Duration, FVector2D OffsetValue, UWorld* World)
// {
// 	if(Widget == nullptr || World == nullptr) return;
// 	
// 	CreateTimedEvent(World);
//
// 	if(ManagerInstance == nullptr) return;
//
// 	FWidgetTransformData* PreviouslyAddedData = FindWidgetTransformData(Widget);
// 	
// 	if(PreviouslyAddedData == nullptr)
// 	{
// 		RemoveWidgetTransformTimer(Widget);
// 		FTimedEvent NewEvent;
// 		LerpWidgetTransform(Widget, Duration, OffsetValue, NewEvent, World, false);
// 	}
// 	else
// 	{
// 		float RemainingDuration = FMath::Clamp(PreviouslyAddedData->Duration - PreviouslyAddedData->Timer, 0.0f, PreviouslyAddedData->Duration);
//
// 		if(RemainingDuration > 0)
// 		{
// 			
// 		}
// 		else
// 		{
// 			FTimedEvent NewEvent;
// 			LerpWidgetTransform(Widget, Duration, OffsetValue, NewEvent, World, false);
// 		}
// 	}
// }

void ATimedEventManager::CloneWidgetMovement(UWidget* From, UWidget* To)
{
	if(From == nullptr || To == nullptr || Instance == nullptr) return;
	if(FindWidgetTransformData(To) != nullptr) return;

	FWidgetTransformData* WidgetDataToClone = FindWidgetTransformData(From);

	if(WidgetDataToClone)
	{
		FWidgetTransformData NewData = *WidgetDataToClone;
		NewData.Widget = To;

		NewData.Queue.Empty();

		for (int i = 0; i < WidgetDataToClone->Queue.Num(); ++i)
		{
			FWidgetTransformData NewQueue = WidgetDataToClone->Queue[i];
			NewQueue.Widget = To;
			NewData.Queue.Add(NewQueue);
		}

		Instance->WidgetTransformData.Add(NewData);
	}
}

void ATimedEventManager::CloneWidgetMovementAsync(UWidget* From, UWidget* To, FString Key)
{
	if(From == nullptr || To == nullptr || Instance == nullptr) return;

	TArray<FWidgetTransformData*> WidgetData;
	FindWidgetTransformAsyncData(From, Key, WidgetData);
	
	if(WidgetData.Num() <= 0) return;

	for (int i = 0; i < WidgetData.Num(); ++i)
	{
		if(WidgetData[i])
		{
			FWidgetTransformData NewQueue = *WidgetData[i];
			NewQueue.Widget = To;
			Instance->WidgetAsyncTransformData.Add(NewQueue);
		}
	}
}

void ATimedEventManager::RemoveWidgetTransformTimer(UWidget* Widget)
{
	if(Instance == nullptr) return;

	for (int i = 0; i < Instance->WidgetTransformData.Num(); ++i)
	{
		if(Instance->WidgetTransformData[i].Widget == Widget)
		{
			Instance->WidgetTransformData.RemoveAt(i);
			return;
		}
	}
}

FWidgetTransformData* ATimedEventManager::FindWidgetTransformData(UWidget* Widget)
{
	if(Instance == nullptr) return nullptr;

	for (int i = 0; i < Instance->WidgetTransformData.Num(); ++i)
	{
		if(Instance->WidgetTransformData[i].Widget == Widget)
		{
			return &Instance->WidgetTransformData[i];
		}
	}

	return nullptr;
}

void ATimedEventManager::FindWidgetTransformAsyncData(UWidget* Widget, FString Key, TArray<FWidgetTransformData*>& OutFoundWidgets)
{
	if(Instance == nullptr) return;

	for (int i = 0; i < Instance->WidgetAsyncTransformData.Num(); ++i)
	{
		if(Instance->WidgetAsyncTransformData[i].Widget == Widget && Key.Equals(Instance->WidgetAsyncTransformData[i].Key))
		{
			OutFoundWidgets.Add(&Instance->WidgetAsyncTransformData[i]);
		}
	}
}

void ATimedEventManager::FindWidgetTransformAsyncData(UWidget* Widget, TArray<FWidgetTransformData*>& OutFoundWidgets)
{
	if(Instance == nullptr) return;

	for (int i = 0; i < Instance->WidgetAsyncTransformData.Num(); ++i)
	{
		if(Instance->WidgetAsyncTransformData[i].Widget == Widget)
		{
			OutFoundWidgets.Add(&Instance->WidgetAsyncTransformData[i]);
		}
	}
}

void ATimedEventManager::LerpCanvasPanelSlotSize(UCanvasPanelSlot* Slot, float Duration, FVector2D StartValue,
	FVector2D EndValue, UWorld* World)
{
	if(Slot == nullptr || World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;
	
	RemoveCanvasPanelSlotSizeTimer(Slot);

	FCanvasPanelSlotSizeData NewData;
	NewData.Slot = Slot;
	NewData.StartValue = StartValue;
	NewData.EndValue = EndValue;
	NewData.Duration = Duration;
	NewData.Timer = 0;
	
	Instance->CanvasPanelSlotSizeData.Add(NewData);
}

void ATimedEventManager::RemoveCanvasPanelSlotSizeTimer(UCanvasPanelSlot* Slot)
{
	if(Instance == nullptr) return;

	for (int i = 0; i < Instance->CanvasPanelSlotSizeData.Num(); ++i)
	{
		if(Instance->CanvasPanelSlotSizeData[i].Slot == Slot)
		{
			Instance->CanvasPanelSlotSizeData.RemoveAt(i);
			return;
		}
	}
}

void ATimedEventManager::SpawnPopupText(EPopupTextType TextType, int Amount, float Duration, FVector SpawnLocation, UWorld* World,  bool Move)
{
	if(World == nullptr) return;
	
	CreateManagerInstance(World);

	if(Instance == nullptr) return;

	if(Instance->PopupTextPool.IsInitialized() == false)
	{
		Instance->PopupTextPool.SetWorld(World);
	}

	if(Instance->PopupTextPool.IsInitialized())
	{
		UPopupTextWidget* PopupWidget = Cast<UPopupTextWidget>(Instance->PopupTextPool.GetOrCreateInstance(AWidgetManager::GetWidget(EWidgetKeys::PopupTextWidget, World)));
		if(PopupWidget && PopupWidget->TextBlock)
		{
			PopupWidget->TextBlock->SetText(Instance->GetTextToPop(TextType, Amount));

			PopupWidget->AddToViewport();

			PopupWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));

			ABattlePlayerController* BattleController = Cast<ABattlePlayerController>(UGameplayStatics::GetPlayerController(World, 0));

			if(BattleController)
			{
				SpawnLocation.X += FMath::RandRange(-20, 20);
				SpawnLocation.Y += FMath::RandRange(-20, 20);
				SpawnLocation.Z += FMath::RandRange(40, 100);
				FVector2D ScreenPos;
				UGameplayStatics::ProjectWorldToScreen(BattleController, SpawnLocation, ScreenPos);
				PopupWidget->SetPositionInViewport(ScreenPos);
				FPopupTextData NewData;
				NewData.Duration = Duration;
				NewData.Timer = 0;
				NewData.Move = Move;
				NewData.SpawnLocation = SpawnLocation;
				NewData.RandomXdir = FMath::RandRange(40, 70) * (1 - 2 *  FMath::RandRange(0, 1));
				
;
				NewData.PopupWidget = PopupWidget;
				NewData.PlayerController = BattleController;
				Instance->PopupTextData.Add(NewData);
			}
		}
	}
}

bool ATimedEventManager::UpdateTextNumber(FTextData& Data, float DeltaTime)
{
	if(Data.TextBlock == nullptr)
	{
		return  false;
	}

	FFormatNamedArguments Args;
	
	Data.Timer += DeltaTime;
	if(Data.Timer >= Data.Duration)
	{
		int NewValue = Data.EndValue;
		Args.Add(Data.ArgumentKey, NewValue);
		const FText FormattedText = FText::Format(
			Data.FormatText,
			Args
		);
		Data.TextBlock->SetText(FormattedText);
		return  false;
	}
	else if(Data.Duration > 0)
	{
		int NewValue = FMath::Lerp(Data.StartValue, Data.EndValue, Data.Timer / Data.Duration);
		Args.Add(Data.ArgumentKey, NewValue);

		const FText FormattedText = FText::Format(
			Data.FormatText,
			Args
		);
		
		Data.TextBlock->SetText(FormattedText);
	}

	return  true;
}

bool ATimedEventManager::UpdateProgressBar(FProgressBarData& Data, float DeltaTime)
{
	if(Data.ProgressBar == nullptr)
	{
		return  false;
	}
	
	Data.Timer += DeltaTime;
	if(Data.Timer >= Data.Duration)
	{
		Data.ProgressBar->SetPercent(Data.EndValue);
		return false;
	}
	else if(Data.Duration > 0)
	{
		float NewPerc = FMath::Lerp(Data.StartValue, Data.EndValue, Data.Timer / Data.Duration);
		Data.ProgressBar->SetPercent(NewPerc);
	}

	return  true;
}

bool ATimedEventManager::UpdateActorRotation(FTargetRotationData& Data, float DeltaTime)
{
	if(Data.OwnerActor == nullptr)
	{
		return  false;
	}
	
	Data.Timer += DeltaTime;
	if(Data.Timer >= Data.Duration)
	{
		Data.OwnerActor->SetActorRotation(Data.TargetRotation);
		return  false;
	}
	else if(Data.Duration > 0)
	{
		FRotator NewRotation = FMath::Lerp(Data.StartRotation, Data.TargetRotation, Data.Timer / Data.Duration);
		Data.OwnerActor->SetActorRotation(NewRotation);
	}
	return  true;
}
bool ATimedEventManager::UpdateActorLocation(FTargetLocationData& Data, float DeltaTime)
{
	if(Data.OwnerActor == nullptr)
	{
		return  false;
	}

	Data.Timer += DeltaTime;
	if(Data.bFollowMode == false)
	{
		bool CheckCondition = true;
		if(Data.ConditionDelegate.IsBound())
		{
			CheckCondition = Data.ConditionDelegate.Execute();
		}

		if(CheckCondition)
		{
			if(Data.Timer >= Data.Duration)
			{
				for (int j = 0; j < Data.OnCompleteDelegate.Num(); ++j)
				{
					if(Data.OnCompleteDelegate[j].IsBound())
					{
						Data.OnCompleteDelegate[j].Execute();
						Data.OnCompleteDelegate[j].Unbind();
					}
				}
			}
			
			if(Data.Timer >= Data.Duration + Data.Delay)
			{
				
				Data.OwnerActor->SetActorLocation(Data.TargetLocation);
				
				return  false;
			}
			else if(Data.Duration > 0 && Data.Timer < Data.Duration)
			{
				FVector NewLocation = FMath::Lerp(Data.StartLocation, Data.TargetLocation, Data.Timer / Data.Duration);
				Data.OwnerActor->SetActorLocation(NewLocation);
			}
		}
		else
		{
			return  false;
		}
		
	}
	else if(Data.ActorToFollow != nullptr)
	{
		bool CheckCondition = true;
		if(Data.ConditionDelegate.IsBound())
		{
			CheckCondition = Data.ConditionDelegate.Execute();
		}

		if(CheckCondition)
		{
			if(Data.Timer >= Data.Duration)
			{
				Data.Timer = Data.Duration;

				FVector FollowPos = Data.ActorToFollow->GetActorLocation();
				FollowPos.Z = Data.OverridenLocationZ;
				FVector NewLocation = FMath::Lerp(Data.OwnerActor->GetActorLocation(), FollowPos, DeltaTime * 7);

				Data.OwnerActor->SetActorLocation(NewLocation);
			}
			else if(Data.Duration > 0)
			{
				FVector FollowPos = Data.ActorToFollow->GetActorLocation();
				FollowPos.Z = Data.OverridenLocationZ;
				FVector NewLocation = FMath::Lerp(Data.StartLocation, FollowPos, Data.Timer / Data.Duration);
				Data.OwnerActor->SetActorLocation(NewLocation);
			}
		}
		else
		{
			return false;
		}
	}

	return  true;
}

bool ATimedEventManager::UpdateWidgetOpacity(FWidgetRenderOpacityData& Data, float DeltaTime)
{
	if(Data.Widget == nullptr)
	{
		return  false;
	}
	
	Data.Timer += DeltaTime;
	if(Data.Timer >= Data.Duration)
	{
		if(Data.IsLooped == false)
		{
			Data.Widget->SetRenderOpacity(Data.EndValue);
			return  false;
		}

		float EndVal = Data.StartValue;
		Data.StartValue = Data.EndValue;
		Data.EndValue = EndVal;
		Data.Timer = 0;
	}
	else if(Data.Duration > 0)
	{
		float NewOpacity = FMath::Lerp(Data.StartValue, Data.EndValue, Data.Timer / Data.Duration);
		Data.Widget->SetRenderOpacity(NewOpacity);
	}
	return  true;
}

bool ATimedEventManager::UpdateWidgetTranslation(FWidgetTransformData& Data, float DeltaTime)
{
	if(Data.Widget == nullptr)
	{
		return  false;
	}
	
	Data.Timer += DeltaTime;
	if(Data.Timer >= Data.Duration)
	{
		Data.Widget->SetRenderTranslation(Data.StartValue + Data.OffsetValue);

		if(Data.Queue.Num() > 0)
		{
			if(Data.OnCompleteDelegate.IsBound())
			{
				Data.OnCompleteDelegate.Execute();
			}
			
			FWidgetTransformData& NewData = Data.Queue[0];
			Data.Duration = NewData.Duration;
			Data.StartValue = Data.Widget->RenderTransform.Translation;
			Data.OffsetValue = NewData.OffsetValue;
			Data.OnCompleteDelegate = NewData.OnCompleteDelegate;
			Data.Timer = 0;
			
			Data.Queue.RemoveAt(0);
		}
		else
		{
			return  false;
		}
	}
	else if(Data.Duration > 0)
	{
		FVector2D NewTransform = FMath::Lerp(Data.StartValue, Data.StartValue + Data.OffsetValue, Data.Timer / Data.Duration);
		Data.Widget->SetRenderTranslation(NewTransform);
	}
	return  true;
}
bool ATimedEventManager::UpdateWidgetTranslationAsync(FWidgetTransformData& Data, float DeltaTime)
{
	if(Data.Widget == nullptr)
	{
		return  false;
	}
	
	if(Data.Timer >= Data.Duration)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Data.FreeVariable1 %s"), *Data.FreeVariable1.ToString());	
		FVector2D NewTransform = Data.Widget->RenderTransform.Translation;
		FVector2D RemainingAmount = Data.OffsetValue - Data.FreeVariable1;
		NewTransform += RemainingAmount;
		Data.Widget->SetRenderTranslation(NewTransform);

		if(Data.Queue.Num() > 0)
		{
			FWidgetTransformData& NewData = Data.Queue[0];
			Data.Duration = NewData.Duration;
			Data.OffsetValue = NewData.OffsetValue;
			Data.Timer = 0;
			Data.FreeVariable1 = FVector2D::ZeroVector;
			
			Data.Queue.RemoveAt(0);
		}
		else
		{
			return  false;
		}
	}
	else if(Data.Duration > 0)
	{
		FVector2D NewTransform = Data.Widget->RenderTransform.Translation;
		FVector2D AmountToAdd = Data.OffsetValue * (DeltaTime / Data.Duration);
		NewTransform += AmountToAdd;
		Data.FreeVariable1 += AmountToAdd;
		Data.Widget->SetRenderTranslation(NewTransform);
	}
	Data.Timer += DeltaTime;
	
	return  true;
}

bool ATimedEventManager::UpdateCanvasPanelSlotSize(FCanvasPanelSlotSizeData& Data, float DeltaTime)
{
	if(Data.Slot == nullptr)
	{
		return  false;
	}
	
	Data.Timer += DeltaTime;
	if(Data.Timer >= Data.Duration)
	{
		Data.Slot->SetSize(Data.EndValue);
		return  false;
	}
	else if(Data.Duration > 0)
	{
		FVector2D NewOpacity = FMath::Lerp(Data.StartValue, Data.EndValue, Data.Timer / Data.Duration);
		Data.Slot->SetSize(NewOpacity);
	}
	return  true;
}

bool ATimedEventManager::UpdatePopupText(FPopupTextData& Data, float DeltaTime)
{
	if(Data.PopupWidget == nullptr || Data.PlayerController == nullptr)
	{
		return  false;
	}

	FVector2D ScreenPos;
	UGameplayStatics::ProjectWorldToScreen(Data.PlayerController, Data.SpawnLocation, ScreenPos);

	FVector2D EndPosition = ScreenPos + FVector2D(Data.RandomXdir, -FMath::Abs(Data.RandomXdir));

	Data.Timer += DeltaTime;
	if(Data.Timer >= Data.Duration)
	{
		if(Data.Move)
		{
			Data.PopupWidget->SetPositionInViewport(EndPosition);
		}
		else
		{
			Data.PopupWidget->SetPositionInViewport(ScreenPos);
		}
		return  false;
	}
	else if(Data.Duration > 0)
	{
		FVector2D NewPosition;
		if(Data.Move)
		{
			NewPosition = FMath::Lerp(ScreenPos, EndPosition, Data.Timer / Data.Duration);
		}
		else
		{
			NewPosition = ScreenPos;
		}
		Data.PopupWidget->SetPositionInViewport(NewPosition);
	}
	return  true;
}

FText ATimedEventManager::GetTextToPop(EPopupTextType PopupType, int Amount)
{
	FFormatNamedArguments Args;
	Args.Add("Amount", Amount);

	FText FormatText;
	if(EPopupTextType::TrueDamage == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "Dmg", "<Red_Big>{Amount}</>");
	}
	else if(EPopupTextType::PhysicalDamage == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "Dmg", "<Orange_Big>{Amount}</>");
	}
	else if(EPopupTextType::MagicalDamage == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "Dmg", "<Blue_Big>{Amount}</>");
	}
	else if(EPopupTextType::Dodged == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "Dodge", "<Orange_Big>Dodged</>");
	}
	else if(EPopupTextType::Lucky == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "Lucky", "<Blue_Big>Lucky Bastard!</>");
	}
	else if(EPopupTextType::Resisted == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "Resisted", "<Orange_Big>Resisted!</>");
	}
	else if(EPopupTextType::Crit == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "Crit", "<Red_Big>CRIT!</>");
	}
	else if(EPopupTextType::MissedEffect == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "MissedEffect", "<Orange_Big>Effect is Missed!</>");
	}
	else if(EPopupTextType::Heal == PopupType)
	{
		FormatText = NSLOCTEXT("CommonWords", "MissedEffect", "<Green_Big>{Amount}</>");
	}
	
	
	const FText FormattedText = FText::Format(
		FormatText,
		Args
	);


	return FormattedText;
}




