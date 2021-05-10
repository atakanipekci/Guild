// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNavNodeActor.h"

#include "GuildGame/Town/TownGameModeBase.h"
#include "TownNpcManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATownNavNodeActor::ATownNavNodeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATownNavNodeActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("NDOE BEGINPLAY "));
	ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode)
	{
		if(GameMode->NpcManager)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ADD TO LIST "));

			GameMode->NpcManager->NavigationNodes.Add(this);
		}
	}
	FHitResult HitResult;
	FVector TraceStartPos(GetActorLocation());
	FVector TraceEndPos(GetActorLocation());
	TraceEndPos.Z -= 1000;
	FCollisionQueryParams CollisionParams;
	
	//UE_LOG(LogTemp, Warning, TEXT("TRACE END %s"), *(TraceEndPos.ToString()));
	
	if(GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartPos, TraceEndPos,ECC_WorldStatic, CollisionParams))
	{
		//UE_LOG(LogTemp, Warning, TEXT("TRACE HIT %s"), *(HitResult.ImpactPoint.ToString()));
		Destination = HitResult.ImpactPoint;
		//Destination.Z += 40;
	}
}

// Called every frame
void ATownNavNodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

