// Fill out your copyright notice in the Description page of Project Settings.


#include "TimedEventManager.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

ATimedEventManager* ATimedEventManager::ManagerInstance = nullptr;

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
	ManagerInstance = nullptr;
}

// Called every frame
void ATimedEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetTickGroup(ETickingGroup::TG_PostPhysics);

	for (int i = RotationData.Num() - 1; i >= 0; --i)
	{
		if(UpdateRotate(RotationData[i], DeltaTime) == false)
		{
			RotationData.RemoveAt(i);
		}
	}

	for (int i = LocationData.Num() - 1; i >= 0; --i)
	{
		if(UpdateMove(LocationData[i], DeltaTime) == false)
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
}

void ATimedEventManager::CreateTimedEvent(UWorld* World)
{
	if(ManagerInstance == nullptr)
	{
		ManagerInstance = World->SpawnActor<ATimedEventManager>();
	}
}

void ATimedEventManager::Rotate(AActor* ActorToRotate, FRotator TargetRotation, float Duration, UWorld* World)
{
	if(ActorToRotate == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
	for (int i = 0; i < ManagerInstance->RotationData.Num(); ++i)
	{
		if(ManagerInstance->RotationData[i].OwnerActor)
		{
			if(ManagerInstance->RotationData[i].OwnerActor == ActorToRotate)
			{
				ManagerInstance->RotationData.RemoveAt(i);
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
	
	ManagerInstance->RotationData.Add(NewData);
}

void ATimedEventManager::Move(AActor* ActorToMove, FVector TargetLocation, float Duration, float Delay, FConditionEvent& ConditionDelegate, TArray<FTimedEvent> OnComplete, FTimedEvent& OnDelayedCompleteDelegate, UWorld* World)
{
	if(ActorToMove == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
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
	
	ManagerInstance->LocationData.Add(NewData);
}

void ATimedEventManager::MoveToActorAndFollow(AActor* ActorToMove, AActor* ActorToFollow, float Duration, float OverridenLocationZ, FConditionEvent& ConditionDelegate, UWorld* World)
{
	if(ActorToMove == nullptr || ActorToFollow == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
	RemoveLocationTimer(ActorToMove);

	FTargetLocationData NewData;
	NewData.bFollowMode = true;
	NewData.OverridenLocationZ = OverridenLocationZ;
	NewData.OwnerActor = ActorToMove;
	NewData.ActorToFollow = ActorToFollow;
	if(ConditionDelegate.IsBound())
	{
		NewData.ConditionDelegate = ConditionDelegate;
	}
	NewData.Duration = Duration;
	NewData.Timer = 0;
	
	ManagerInstance->LocationData.Add(NewData);
}

void ATimedEventManager::RemoveLocationTimer(AActor* ActorToMove)
{
	if(ManagerInstance == nullptr) return;
	
	for (int i = 0; i < ManagerInstance->LocationData.Num(); ++i)
	{
		if(ManagerInstance->LocationData[i].OwnerActor == ActorToMove)
		{
			FTargetLocationData DataToRemove = ManagerInstance->LocationData[i];
			ManagerInstance->LocationData.RemoveAt(i);

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
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;

	RemoveEventData(Key, true);
	FTimerEventData EventData;
	EventData.Duration = Duration;
	EventData.Timer = 0;
	EventData.OwnerActor = EventActor;
	if(EventToCall.IsBound())
	{
		EventData.OnTimeEnds = EventToCall;
	}
	ManagerInstance->TimedEventMap.Add(Key, EventData);
}

bool ATimedEventManager::RemoveEventData(FString Key, bool bCallEvent)
{
	if(ManagerInstance == nullptr) return false;

	if(ManagerInstance->TimedEventMap.Contains(Key))
	{
		FTimerEventData EventData;
		ManagerInstance->TimedEventMap.RemoveAndCopyValue(Key, EventData);

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
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
	for (int i = 0; i < ManagerInstance->ProgressBarData.Num(); ++i)
	{
		if(ManagerInstance->ProgressBarData[i].ProgressBar)
		{
			if(ManagerInstance->ProgressBarData[i].ProgressBar == ProgressBar)
			{
				ManagerInstance->ProgressBarData.RemoveAt(i);
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
	
	ManagerInstance->ProgressBarData.Add(NewData);
}

void ATimedEventManager::LerpTextNumber(UTextBlock* TextBlock, FText FormatText, FString ArgumentKey, float Duration,
	float StartValue, float EndValue, UWorld* World)
{
	if(TextBlock == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
	for (int i = 0; i < ManagerInstance->TextData.Num(); ++i)
	{
		if(ManagerInstance->TextData[i].TextBlock)
		{
			if(ManagerInstance->TextData[i].TextBlock == TextBlock)
			{
				ManagerInstance->TextData.RemoveAt(i);
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
	
	ManagerInstance->TextData.Add(NewData);
}

void ATimedEventManager::LerpWidgetOpacity(UWidget* Widget, float Duration, float StartValue, float EndValue,
	bool IsLooped, UWorld* World)
{
	if(Widget == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
	for (int i = 0; i < ManagerInstance->WidgetOpacityData.Num(); ++i)
	{
		if(ManagerInstance->WidgetOpacityData[i].Widget)
		{
			if(ManagerInstance->WidgetOpacityData[i].Widget == Widget)
			{
				ManagerInstance->WidgetOpacityData.RemoveAt(i);
				break;
			}
		}
	}

	FWidgetRenderOpacityData NewData;
	NewData.Widget = Widget;
	NewData.StartValue = StartValue;
	NewData.EndValue = EndValue;
	NewData.Duration = Duration;
	NewData.IsLooped = IsLooped;
	NewData.Timer = 0;
	
	ManagerInstance->WidgetOpacityData.Add(NewData);
}

void ATimedEventManager::RemoveWidgetOpacityTimer(UWidget* Widget)
{
	if(ManagerInstance == nullptr) return;

	for (int i = 0; i < ManagerInstance->WidgetOpacityData.Num(); ++i)
	{
		if(ManagerInstance->WidgetOpacityData[i].Widget == Widget)
		{
			ManagerInstance->WidgetOpacityData.RemoveAt(i);
			return;
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

bool ATimedEventManager::UpdateRotate(FTargetRotationData& Data, float DeltaTime)
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
bool ATimedEventManager::UpdateMove(FTargetLocationData& Data, float DeltaTime)
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
				Data.OwnerActor->SetActorLocation(FollowPos);
			}
			else if(Data.Duration > 0)
			{
				FVector FollowPos = Data.ActorToFollow->GetActorLocation();
				FollowPos.Z = Data.OverridenLocationZ;
				FVector NewLocation = FMath::Lerp(Data.OwnerActor->GetActorLocation(), FollowPos, Data.Timer / Data.Duration);
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




