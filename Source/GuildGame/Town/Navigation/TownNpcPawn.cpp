// Fill out your copyright notice in the Description page of Project Settings.


#include "TownNpcPawn.h"

//#include "NavigationSystem.h"
#include "AIController.h"
#include "TownNpcAIController.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ATownNpcPawn::ATownNpcPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMesh"));
	SkeletalMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	//SkeletalMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	//ItemSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("movement component"));
	MovementComponent->UpdatedComponent = RootComponent;


	AutoPossessAI = EAutoPossessAI::Spawned;
}

void ATownNpcPawn::MoveActor(FVector TargetPos)
{
	int intVar = TargetPos.X;
	float floatVar = 3.7f;
	FString fstringVar = "MoveUnitsToPosition IF";
	
	UE_LOG(LogTemp, Warning, TEXT("MoveActor"));

	TArray<FVector> Locations;
	// if (NavSystem)
	// {
		if(AiController != nullptr)
		{
			//AiController->MoveTo(MoveReq , &OutPath);

			AiController->MoveToLocation(TargetPos);
			FString fstringVar2 = "IS FALLOWING PATH ";
			UE_LOG(LogTemp, Warning, TEXT("Text, %d  %s"), AiController->IsFollowingAPath(), *fstringVar2 );
		}
	//}
}
// Called when the game starts or when spawned
void ATownNpcPawn::BeginPlay()
{
	Super::BeginPlay();

	//UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	//NavSystem->effe
	ATownNpcAIController* AIController = Cast<ATownNpcAIController>(GetController());
	if(AIController)
	{
		AIController->MoveToLocation(FVector(2460.0f, -2170.0f, -1281.248779f),5,false,true,false,true,0,false);
	}
		
	//MoveActor(FVector(2460.0f, -2170.0f, -1281.248779f));

}

// Called every frame
void ATownNpcPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("IS Cllision Enabled, %d "), 	SkeletalMesh->CanEverAffectNavigation());
}

// Called to bind functionality to input
void ATownNpcPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

