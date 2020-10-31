// Fill out your copyright notice in the Description page of Project Settings.


#include "GGCameraSpectatorPawn.h"

#include "GGPlayerController.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "Engine/Classes/Engine/Engine.h"
#include "GGControllerState.h"
#include "Kismet/GameplayStatics.h"

AGGCameraSpectatorPawn::AGGCameraSpectatorPawn(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bAddDefaultMovementBindings = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bCanMoveCamera = true;

	// collision
	GetCollisionComponent()->SetGenerateOverlapEvents(false);
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCollisionComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCollisionComponent()->SetSimulatePhysics(false);

	CameraXYLimit = 7500.f;
	CameraHeight = 300.f;
	CameraHeightMin = 5.f;
	CameraHeightMax = 5000.f;

	CameraRadius = 1100.f;
	CameraRadiusMin = 100.f;
	CameraRadiusMax = 8000.f;

	CameraZAngle = 0.f;
	CameraHeightAngle = 50.f;                // pitch
	CameraHeightAngleMin = 15.f;
	CameraHeightAngleMax = 85.f;


	CameraZoomSpeed = 200.f;
	CameraMovementSpeed = 3000.f;
	CameraRotationSpeed = 10.f;

	CameraScrollBoundary = 30.f; // screen edge margin

	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopCamera"));
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->bUsePawnControlRotation = false;
	RepositionCamera();
}

void AGGCameraSpectatorPawn::LeftClickHandler()
{
	AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}

	if(PlayerController->GetActiveState())
	{
		PlayerController->GetActiveState()->LeftClickHandler();
	}
}

void AGGCameraSpectatorPawn::LeftClickReleaseHandler()
{
	AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}

	if(PlayerController->GetActiveState())
	{
		PlayerController->GetActiveState()->LeftClickReleaseHandler();
	}
}

void AGGCameraSpectatorPawn::RightClickHandler()
{
	AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}
	if(PlayerController->GetActiveState())
	{
		PlayerController->GetActiveState()->RightClickHandler();
	}
}

void AGGCameraSpectatorPawn::RightClickReleaseHandler()
{
	AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}

	if(PlayerController->GetActiveState())
	{
		PlayerController->GetActiveState()->RightClickReleaseHandler();
	}
}


void AGGCameraSpectatorPawn::RotateInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	RotateValue = FMath::Abs(Direction);
}

void AGGCameraSpectatorPawn::ZoomInByWheel()
{
	if (!bCanMoveCamera) 
	{
		return;
	}

	CameraRadius -= CameraZoomSpeed;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void AGGCameraSpectatorPawn::ZoomOutByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraRadius += CameraZoomSpeed;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void AGGCameraSpectatorPawn::RotateLeftByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraZAngle -= CameraRotationSpeed;
}

void AGGCameraSpectatorPawn::RotateRightByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraZAngle += CameraRotationSpeed;
}

void AGGCameraSpectatorPawn::RotateUpByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

	CameraHeightAngle += CameraRotationSpeed;
	CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void AGGCameraSpectatorPawn::RotateDownByWheel()
{
	if (!bCanMoveCamera)
	{
		return;
	}

    CameraHeightAngle -= CameraRotationSpeed;
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void AGGCameraSpectatorPawn::MoveCameraForwardInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	MoveForwardValue = Direction;
}

void AGGCameraSpectatorPawn::MoveCameraRightInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	MoveRightValue = Direction;
}

void AGGCameraSpectatorPawn::MoveCameraUpInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	MoveUpValue = Direction;
}

void AGGCameraSpectatorPawn::ZoomCameraInInput(float Direction)
{
	if (!bCanMoveCamera)
	{
		return;
	}

	ZoomInValue = Direction;
}

void AGGCameraSpectatorPawn::RepositionCamera()
{
	FVector NewLocation(0.f, 0.f, 0.f);
	FRotator NewRotation(0.f, 0.f, 0.f);

	float sinCameraZAngle = FMath::Sin(FMath::DegreesToRadians(CameraZAngle));
	float cosCameraZAngle = FMath::Cos(FMath::DegreesToRadians(CameraZAngle));

	float sinCameraHeightAngle = FMath::Sin(FMath::DegreesToRadians(CameraHeightAngle));
	float cosCameraHeightAngle = FMath::Cos(FMath::DegreesToRadians(CameraHeightAngle));

	NewLocation.X = cosCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Y = sinCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Z = sinCameraHeightAngle * CameraRadius;
	
	/*float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(GetActorLocation().X + NewLocation.X, GetActorLocation().Y + NewLocation.Y);
	if (GetActorLocation().Z + NewLocation.Z < TerrainSurfaceZ + CameraHeight)
	{
		NewLocation.Z = TerrainSurfaceZ - GetActorLocation().Z + CameraHeight;
	}*/

	CameraComponent->SetRelativeLocation(NewLocation);
	NewRotation = (FVector(0.0f, 0.0f, 0.0f) - NewLocation).Rotation();
	CameraComponent->SetRelativeRotation(NewRotation);
}

void AGGCameraSpectatorPawn::No1Clicked()
{
	AGGPlayerController* PlayerController = Cast<AGGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController == nullptr)
	{
		return;
	}

	PlayerController->ChangeStateTo(1);
}

float AGGCameraSpectatorPawn::GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord) const
{
	FCollisionQueryParams TraceParams(FName(TEXT("LandTerrain")), false, this);        // TraceTag (info for debugging), bTraceComplex, AddIgnoredActor
	TraceParams.bFindInitialOverlaps = false;

	FHitResult Hit;

	FVector Start = FVector(XCoord, YCoord, GetActorLocation().Z + CameraRadius);
	FVector End = FVector(XCoord, YCoord, -500.f);

	// ECC_ channels should be set properly !!!
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, TraceParams);

	if (bHit)
	{
		return Hit.ImpactPoint.Z;    // for shape trace it differs from Location
	}

	return 0.f;        // water level
}

void AGGCameraSpectatorPawn::Tick(float DeltaSeconds)
{
		Super::Tick(DeltaSeconds);

	FVector2D MousePosition;
	FVector2D ViewportSize;
	
	if (!GEngine)
	{
		return;
	}

	UGameViewportClient* GameViewport = GEngine->GameViewport;

	// it is always nullptr on dedicated server
	if (!GameViewport)
	{
		return;
	}

	GameViewport->GetViewportSize(ViewportSize);

	// if viewport is focused, contains the mouse, and camera movement is allowed
	if (GameViewport->IsFocused(GameViewport->Viewport)
		&& GameViewport->GetMousePosition(MousePosition) && bCanMoveCamera)
	{
		if (MousePosition.X < CameraScrollBoundary)
		{
			MoveRightValue = -1.0f;
		}
		else if (ViewportSize.X - MousePosition.X < CameraScrollBoundary)
		{
			MoveRightValue = 1.0f;
		}

		if (MousePosition.Y < CameraScrollBoundary)
		{
			MoveForwardValue = 1.0f;
		}
		else if (ViewportSize.Y - MousePosition.Y < CameraScrollBoundary)
		{
			MoveForwardValue = -1.0f;
		}

		FVector ActualLocation = GetActorLocation();
		FVector ActualMovement = FVector::ZeroVector;
	
		if (RotateValue == 0.f)
		{
			ActualMovement += MoveCameraForward(MoveForwardValue * DeltaSeconds);
			ActualMovement += MoveCameraRight(MoveRightValue * DeltaSeconds);
		}
		ActualLocation += ActualMovement;

		CameraHeight += MoveCameraUp(MoveUpValue * DeltaSeconds);
		CameraHeight = FMath::Clamp(CameraHeight, CameraHeightMin, CameraHeightMax);

		//float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(ActualLocation.X, ActualLocation.Y);
		ActualLocation.Z = CameraHeight;

		ActualLocation.X = FMath::Clamp(ActualLocation.X, -CameraXYLimit, CameraXYLimit);
		ActualLocation.Y = FMath::Clamp(ActualLocation.Y, -CameraXYLimit, CameraXYLimit);

		SetActorLocation(ActualLocation);

		if (RotateValue != 0.f)
		{
			TurnCameraUp(MoveForwardValue * DeltaSeconds);
			TurnCameraRight(MoveRightValue * DeltaSeconds);
		}

		ZoomCameraIn(ZoomInValue * DeltaSeconds);
		RepositionCamera();
	}
}

FVector AGGCameraSpectatorPawn::MoveCameraForward(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * GetIsolatedCameraYaw().Vector();
	return DeltaMovement;
}

FVector AGGCameraSpectatorPawn::MoveCameraRight(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * (FRotator(0.0f, 90.0f, 0.0f) + GetIsolatedCameraYaw()).Vector();
	return DeltaMovement;
}


float AGGCameraSpectatorPawn::MoveCameraUp(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	return MovementValue;
}

void AGGCameraSpectatorPawn::ZoomCameraIn(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	CameraRadius += MovementValue;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}

void AGGCameraSpectatorPawn::TurnCameraUp(float Direction)
{
	CameraHeightAngle -= Direction * CameraRotationSpeed * 10.0f;
	CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

void AGGCameraSpectatorPawn::TurnCameraRight(float Direction)
{
	CameraZAngle += Direction * CameraRotationSpeed * 10.0f;
}

FRotator AGGCameraSpectatorPawn::GetIsolatedCameraYaw()
{
	return FRotator(0.0f, CameraComponent->GetComponentToWorld().Rotator().Yaw, 0.0f);
}

void AGGCameraSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent)
	{
		return;
	}

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &AGGCameraSpectatorPawn::ZoomOutByWheel);
	PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &AGGCameraSpectatorPawn::ZoomInByWheel);
	PlayerInputComponent->BindAction("RotateUpByWheel", IE_Pressed, this, &AGGCameraSpectatorPawn::RotateUpByWheel);
	PlayerInputComponent->BindAction("RotateDownByWheel", IE_Pressed, this, &AGGCameraSpectatorPawn::RotateDownByWheel);
	PlayerInputComponent->BindAction("RotateLeftByWheel", IE_Pressed, this, &AGGCameraSpectatorPawn::RotateLeftByWheel);
	PlayerInputComponent->BindAction("RotateRightByWheel", IE_Pressed, this, &AGGCameraSpectatorPawn::RotateRightByWheel);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGGCameraSpectatorPawn::MoveCameraForwardInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGGCameraSpectatorPawn::MoveCameraRightInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &AGGCameraSpectatorPawn::MoveCameraUpInput);
	PlayerInputComponent->BindAxis("ZoomIn", this, &AGGCameraSpectatorPawn::ZoomCameraInInput);
	PlayerInputComponent->BindAxis("Rotate", this, &AGGCameraSpectatorPawn::RotateInput);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AGGCameraSpectatorPawn::LeftClickHandler);
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &AGGCameraSpectatorPawn::LeftClickReleaseHandler);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AGGCameraSpectatorPawn::RightClickHandler);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &AGGCameraSpectatorPawn::RightClickReleaseHandler);
	PlayerInputComponent->BindAction("No1Click", IE_Pressed, this, &AGGCameraSpectatorPawn::No1Clicked);
}