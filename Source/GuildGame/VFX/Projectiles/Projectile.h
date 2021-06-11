// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStaticsTypes.h"

#include "Projectile.generated.h"

UCLASS()
class GUILDGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<class AGGCharacter*> SelectedTargetCharacters;
	UPROPERTY()
	class AGGCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	float Angle;
	UPROPERTY(EditAnywhere)
	float Speed;

	FPredictProjectilePathResult ProjectileResult;
	int PathIndex = 0;

	bool bStartLaunch = false;

	bool bProjectileDeactivated = false;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;

	void SetVelocityViaTarget(FVector LocationOfTarget);

	static FVector GetVelocityVector(FVector StartLocation, FVector TargetLocation, float Angle, UWorld* World);

	static bool TraceTrajectory(FVector StartPosition, FVector TargetPosition, float Angle, UWorld* World, struct FPredictProjectilePathResult& ProjectileResult, TArray<AActor*> ActorsToIgnore, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToHit);

	UFUNCTION()
	void OnCollisionOverlapBegin(class UPrimitiveComponent* OverlappedComp,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	void OnSkillHits(FVector Normal);
};
