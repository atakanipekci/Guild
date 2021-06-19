// Fill out your copyright notice in the Description page of Project Settings.


#include "TimedEventManager.h"

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

	for (int i = 0; i < RotationData.Num(); ++i)
	{
		if(RotationData[i].OwnerActor == nullptr)
		{
			RotationData.RemoveAt(i);
			i--;
			i = FMath::Max(0, i);
		}
		else
		{
			RotationData[i].Timer += DeltaTime;
			if(RotationData[i].Timer >= RotationData[i].Duration)
			{
				RotationData[i].OwnerActor->SetActorRotation(RotationData[i].TargetRotation);
				RotationData.RemoveAt(i);
				i--;
				i = FMath::Max(0, i);
			}
			else if(RotationData[i].Duration > 0)
			{
				FRotator NewRotation = FMath::Lerp(RotationData[i].StartRotation, RotationData[i].TargetRotation, RotationData[i].Timer / RotationData[i].Duration);
				RotationData[i].OwnerActor->SetActorRotation(NewRotation);
			}
		}
	}

	for (int i = 0; i < LocationData.Num(); ++i)
	{
		if(LocationData[i].OwnerActor == nullptr)
		{
			LocationData.RemoveAt(i);
			i--;
			i = FMath::Max(0, i);
		}
		else
		{
			LocationData[i].Timer += DeltaTime;
			if(LocationData[i].bFollowMode == false)
			{
				bool CheckCondition = true;
				if(LocationData[i].ConditionDelegate.IsBound())
				{
					CheckCondition = LocationData[i].ConditionDelegate.Execute();
				}

				if(CheckCondition)
				{
					if(LocationData[i].Timer >= LocationData[i].Duration)
					{
						LocationData[i].OwnerActor->SetActorLocation(LocationData[i].TargetLocation);
						LocationData.RemoveAt(i);
						i--;
						i = FMath::Max(0, i);
					}
					else if(LocationData[i].Duration > 0)
					{
						FVector NewLocation = FMath::Lerp(LocationData[i].StartLocation, LocationData[i].TargetLocation, LocationData[i].Timer / LocationData[i].Duration);
						LocationData[i].OwnerActor->SetActorLocation(NewLocation);
					}
				}
				else
				{
					LocationData.RemoveAt(i);
					i--;
					i = FMath::Max(0, i);
				}
				
			}
			else if(LocationData[i].ActorToFollow != nullptr)
			{
				bool CheckCondition = true;
				if(LocationData[i].ConditionDelegate.IsBound())
				{
					CheckCondition = LocationData[i].ConditionDelegate.Execute();
				}

				if(CheckCondition)
				{
					if(LocationData[i].Timer >= LocationData[i].Duration)
					{
						LocationData[i].Timer = LocationData[i].Duration;

						FVector FollowPos = LocationData[i].ActorToFollow->GetActorLocation();
						FollowPos.Z = LocationData[i].OverridenLocationZ;
						LocationData[i].OwnerActor->SetActorLocation(FollowPos);
					}
					else if(LocationData[i].Duration > 0)
					{
						FVector FollowPos = LocationData[i].ActorToFollow->GetActorLocation();
						FollowPos.Z = LocationData[i].OverridenLocationZ;
						FVector NewLocation = FMath::Lerp(LocationData[i].OwnerActor->GetActorLocation(), FollowPos, LocationData[i].Timer / LocationData[i].Duration);
						LocationData[i].OwnerActor->SetActorLocation(NewLocation);
					}
				}
				else
				{
					LocationData.RemoveAt(i);
					i--;
					i = FMath::Max(0, i);
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

void ATimedEventManager::Move(AActor* ActorToMove, FVector TargetLocation, float Duration, FConditionEvent& ConditionDelegate, UWorld* World)
{
	if(ActorToMove == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
	for (int i = 0; i < ManagerInstance->LocationData.Num(); ++i)
	{
		if(ManagerInstance->LocationData[i].OwnerActor)
		{
			if(ManagerInstance->LocationData[i].OwnerActor == ActorToMove)
			{
				ManagerInstance->LocationData.RemoveAt(i);
				break;
			}
		}
	}

	FTargetLocationData NewData;
	NewData.bFollowMode = false;
	NewData.OwnerActor = ActorToMove;
	NewData.StartLocation = ActorToMove->GetActorLocation();
	NewData.TargetLocation = TargetLocation;
	if(ConditionDelegate.IsBound())
	{
		NewData.ConditionDelegate = ConditionDelegate;
	}
	NewData.ActorToFollow = nullptr;
	NewData.Duration = Duration;
	NewData.Timer = 0;
	
	ManagerInstance->LocationData.Add(NewData);
}

void ATimedEventManager::MoveToActorAndFollow(AActor* ActorToMove, AActor* ActorToFollow, float Duration, float OverridenLocationZ, FConditionEvent& ConditionDelegate, UWorld* World)
{
	if(ActorToMove == nullptr || ActorToFollow == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;
	
	for (int i = 0; i < ManagerInstance->LocationData.Num(); ++i)
	{
		if(ManagerInstance->LocationData[i].OwnerActor)
		{
			if(ManagerInstance->LocationData[i].OwnerActor == ActorToMove)
			{
				ManagerInstance->LocationData.RemoveAt(i);
				break;
			}
		}
	}

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

void ATimedEventManager::CallEventWithDelay(AActor* EventActor, FString Key, FTimedEvent& EventToCall, float Duration, UWorld* World)
{
	if(EventActor == nullptr || World == nullptr) return;
	
	CreateTimedEvent(World);

	if(ManagerInstance == nullptr) return;

	if(RemoveEventData(Key, true) == false)
	{
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

