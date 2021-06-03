// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	if(ProjectileMovementComponent)
	{
		//ProjectileMovementComponent->ProjectileGravityScale = 0;
	}

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	if(SphereComponent)
	{
		FCollisionResponseContainer Response;
		Response.SetAllChannels(ECollisionResponse::ECR_Ignore);
		Response.SetResponse(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
		Response.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Overlap);
		Response.SetResponse(ECC_Destructible, ECollisionResponse::ECR_Overlap);

		SphereComponent->SetCollisionResponseToChannels(Response);
	}
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	DestroyTimer = 0;
	
	if(SphereComponent)
	{
		SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectile::OnCollisionOverlapBegin);
	}
	// if(SelectedTargetCharacters.Num() <= 0)
	// {
	// 	if(OwnerCharacter)
	// 	{
	// 		//OwnerCharacter->OnAttackHitsEnemy();
	// 		OwnerCharacter->OnCastingSkillEnds();
	// 		Destroy();
	// 	}
	// }
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestroyTimer += DeltaTime;
	if(DestroyTimer >=10)
	{
		Destroy();
	}

}

void AProjectile::SetVelocityViaTarget(FVector LocationOfTarget)
{
	FVector StartPosition = GetActorLocation();
	FVector TargetPosition = LocationOfTarget;
    SetActorRotation(FRotator::ZeroRotator);


	FVector VelocityOutput = GetVelocityVector(StartPosition, TargetPosition, Angle, GetWorld());
			
	ProjectileMovementComponent->SetVelocityInLocalSpace(VelocityOutput);
}

FVector AProjectile::GetVelocityVector(FVector StartLocation, FVector TargetLocation, float Angle, UWorld* World)
{
	//  == Fixed Angle Projectile Velocity Calculation == 

	const float Gravity = World->GetGravityZ() * -1; // Gravity. (Must be a positive value)
	//const float Gravity = 980.f; // This is the same as the line above (unless your project settings have been changed)
	const float Theta = (Angle * PI / 180); // Launch angle in radians (40 being the launch angle in degrees)

	FVector Dir = TargetLocation - StartLocation; //direction
	float Sz = Dir.Z; // Height difference
	Dir.Z = 0; // Remove hight from direction
	float Sx = Dir.Size(); // Distance

	const float V = (Sx / cos(Theta)) * FMath::Sqrt((Gravity * 1) / (2 * (Sx * tan(Theta) - Sz)));
	FVector VelocityOutput = FVector(V*cos(Theta), 0, V*sin(Theta));
	const FRotator Newrot = UKismetMathLibrary::FindLookAtRotation( StartLocation, TargetLocation);
	VelocityOutput = VelocityOutput.RotateAngleAxis(Newrot.Yaw, FVector::ZAxisVector);

	return VelocityOutput;
}

bool AProjectile::TraceTrajectory(FVector StartPosition, FVector TargetPosition, float Angle, UWorld* World, FPredictProjectilePathResult& ProjectileResult, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHit)
{
	FVector VelocityOutput = GetVelocityVector(StartPosition, TargetPosition, Angle, World);
	
	FPredictProjectilePathParams ProjectileParams;
	
	
	ProjectileParams.StartLocation = StartPosition;
	ProjectileParams.LaunchVelocity = VelocityOutput;
	ProjectileParams.ProjectileRadius = 1;
	ProjectileParams.MaxSimTime = 2;
	ProjectileParams.DrawDebugType = EDrawDebugTrace::None;
	ProjectileParams.bTraceWithCollision = true;
	ProjectileParams.bTraceWithChannel = false;

	ProjectileParams.ActorsToIgnore = ActorsToIgnore;
	
	ProjectileParams.ObjectTypes = ObjectTypesToHit;

	

	return UGameplayStatics::PredictProjectilePath(World, ProjectileParams, ProjectileResult);
}

void AProjectile::OnCollisionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGGCharacter* OverlapedChar = Cast<AGGCharacter>(OtherActor);
	if(OverlapedChar)
	{
		if(SelectedTargetCharacters.Contains(OverlapedChar))
		{
			if(OwnerCharacter)
			{
				OwnerCharacter->OnAttackHitsEnemy(OverlapedChar);
				OwnerCharacter->OnCastingSkillEnds();
				Destroy();
				FRotator HitRotation = UKismetMathLibrary::MakeRotFromX(SweepResult.Normal);
				UGameplayStatics::SpawnEmitterAtLocation(this, HitEffect, GetActorLocation(), HitRotation, true);
				//SetLife
			}
		}
	}
}



