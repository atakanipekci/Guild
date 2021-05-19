// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"
#include "TownBuildingInteractionManager.h"

ATownPlayerController::ATownPlayerController()
{
}

void ATownPlayerController::BeginPlay()
{

	BuildingInteractionManager = NewObject<UTownBuildingInteractionManager>();
	if(BuildingInteractionManager)
		BuildingInteractionManager->ManuelConstructor(EInteractionStatee::BuildingSelection, this);
}

void ATownPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	
    if(BuildingInteractionManager)
    {
        BuildingInteractionManager->Tick();
    }
}

void ATownPlayerController::InteractionControllerWidgetToggle()
{
    if(BuildingInteractionManager)
    {
        BuildingInteractionManager->ToggleWidget();
    }
}
void ATownPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &AMyDefaultPawn::ZoomOutByWheel);
	// PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &AMyDefaultPawn::ZoomInByWheel);
	// PlayerInputComponent->BindAction("RotateUpByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateUpByWheel);
	// PlayerInputComponent->BindAction("RotateDownByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateDownByWheel);
	// PlayerInputComponent->BindAction("RotateLeftByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateLeftByWheel);
	// PlayerInputComponent->BindAction("RotateRightByWheel", IE_Pressed, this, &AMyDefaultPawn::RotateRightByWheel);
	// PlayerInputComponent->BindAxis("MoveForward", this, &AMyDefaultPawn::MoveCameraForwardInput);
	// PlayerInputComponent->BindAxis("MoveRight", this, &AMyDefaultPawn::MoveCameraRightInput);
	// PlayerInputComponent->BindAxis("MoveUp", this, &AMyDefaultPawn::MoveCameraUpInput);
	// PlayerInputComponent->BindAxis("ZoomIn", this, &AMyDefaultPawn::ZoomCameraInInput);
	// PlayerInputComponent->BindAxis("Rotate", this, &AMyDefaultPawn::RotateInput);
	//PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATownDefaultPawn::CreateWidgetViaCode);
	//PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &AMyDefaultPawn::LeftClickReleaseHandler);
	// PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &AMyDefaultPawn::RightClickReleaseHandler);

	 InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATownPlayerController::LeftClickHandler);
	 //InputComponent->BindAction("RightMouseClick", IE_Pressed, InteractionController, &UTownInteractionController::ZoomOutFromBuilding);

}

void ATownPlayerController::LeftClickHandler()
{
		if(BuildingInteractionManager)
		{
			BuildingInteractionManager->LeftClickHandler();
		}
}

void ATownPlayerController::BeginDestroy()
{
	Super::BeginDestroy();
	if(BuildingInteractionManager)
	{
		BuildingInteractionManager->ConditionalBeginDestroy();
	}
}




