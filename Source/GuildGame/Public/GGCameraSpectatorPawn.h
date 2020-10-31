// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "GGCameraSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class GUILDGAME_API AGGCameraSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	public:

	AGGCameraSpectatorPawn(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraXYLimit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraHeightMin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraHeightMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraZAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraHeightAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraHeightAngleMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraHeightAngleMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraRadiusMax;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraRadiusMin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraZoomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraMovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraScrollBoundary;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bCanMoveCamera;

private:

	float MoveForwardValue;
	float MoveRightValue;
	float MoveUpValue;
	float ZoomInValue;
	float RotateValue;

public:

	void LeftClickHandler();

	void LeftClickReleaseHandler();

	void RightClickHandler();
	
	void RightClickReleaseHandler();

	void RotateInput(float Direction);

	void ZoomInByWheel();

	void ZoomOutByWheel();

	void RotateLeftByWheel();

	void RotateRightByWheel();

	void RotateUpByWheel();

	void RotateDownByWheel();

	void MoveCameraForwardInput(float Direction);

	void MoveCameraRightInput(float Direction);

	void MoveCameraUpInput(float Direction);

	void ZoomCameraInInput(float Direction);

	void RepositionCamera();

	void No1Clicked();
	
	float GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord)const;

	virtual void Tick(float DeltaSeconds)override;

private:

	FVector MoveCameraForward(float Direction);

	FVector MoveCameraRight(float Direction);

	FRotator GetIsolatedCameraYaw();

	float MoveCameraUp(float Direction);

	void ZoomCameraIn(float Direction);

	void TurnCameraUp(float Direction);

	void TurnCameraRight(float Direction);

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

};
