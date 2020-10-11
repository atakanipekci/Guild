// Fill out your copyright notice in the Description page of Project Settings.


#include "TownDefaultPawn.h"



#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "TownBuildingActorComponent.h"
#include "TownPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATownDefaultPawn::ATownDefaultPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

ATownDefaultPawn::ATownDefaultPawn(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	if(SpringArmComponent)
	{
		SpringArmComponent->SetupAttachment(RootComponent);
	}

	if(Camera)
		Camera->SetupAttachment(SpringArmComponent);
}

void ATownDefaultPawn::LeftClickHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Mouse Clicked"));
	ATownPlayerController* PlayerController = Cast<ATownPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (PlayerController != nullptr)
	{
		FHitResult Hit;
		PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, Hit);
		if (Hit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("hit"));
			if(Hit.Actor != nullptr)
			{
				UActorComponent* Building = Hit.Actor->GetComponentByClass(UTownBuildingActorComponent::StaticClass());
				if(Building != nullptr)
				{
					if(bIsBuildingFocused == false && bEnableInput == true)
					{
						SelectedBuilding = Cast<UTownBuildingActorComponent>(Building);
						SequenceAsset = SelectedBuilding->GetSequenceAsset();
						if(SequenceAsset)
							UE_LOG(LogTemp, Warning, TEXT("Play asset On , %s"),*(Hit.Actor->GetName()));

						PlaySequence((Hit.Actor));
					}
				}
			}
			UE_LOG(LogTemp, Warning, TEXT("RAYCAST ITEM, %s"),*(Hit.Actor->GetName()));
		}	
	}
}

void ATownDefaultPawn::RightClickHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("Right Mouse Clicked"));

	ATownPlayerController* PlayerController = Cast<ATownPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (PlayerController != nullptr)
	{
		if(bIsBuildingFocused && bEnableInput == true && SequenceAsset != nullptr)
		{
			PlaySequenceReverse();
		}
	}
}

void ATownDefaultPawn::PlaySequence(TWeakObjectPtr<AActor> Actor)
{
	if(SequenceAsset == nullptr)
		return;
	
	//if (SequencePlayer == nullptr && SequenceActor == nullptr)
	{
          SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SequenceAsset, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	}
	
     
                 //Sequence Play
           if (SequencePlayer)
           {
           	  SequenceActor->LevelSequence = SequenceAsset;
           	  bIsBuildingFocused = true;
           	  SequencePlayer->Stop();
              SequencePlayer->Play();
           	  bEnableInput = false;
           	  SequencePlayer->OnFinished.AddUniqueDynamic(this, &ATownDefaultPawn::EnableInputOnFinish);
           }
}

void ATownDefaultPawn::PlaySequenceReverse()
{
	if (SequenceAsset && SequencePlayer && SequenceActor)
	{
	   SequenceActor->LevelSequence = SequenceAsset;
       bIsBuildingFocused = false;
	   SequencePlayer->Stop();
       SequencePlayer->PlayReverse();
	   bEnableInput = false;
	   SequencePlayer->OnFinished.AddUniqueDynamic(this, &ATownDefaultPawn::EnableInputOnFinish);
	}
}

void ATownDefaultPawn::IsWidgetCreated()
{
	/*if(SelectedBuilding)
	{
		AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(this));
		if(GameMode)
		{
			//UUserWidget* Widget = GameMode->GetMappedWidget(SelectedBuilding->BuildingDataKey);

			if(Widget == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("WIDGET IS NULL"));

				 Widget = CreateWidget<UMyUserWidget>(this, UMyUserWidget::StaticClass());
				 FInputModeGameAndUI Mode;
				 Mode.SetLockMouseToViewport(true);
				 Mode.SetHideCursorDuringCapture(false);
				 SetInputMode(Mode);
				 UserInterface->AddToViewport(9999); // Z-order, this just makes it render on the very top.
				 
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("WIDGET IS FIRE"));
				 
				 UUserWidget* WidgetInstance = NewObject<UUserWidget>(Widget);
				 FInputModeGameAndUI Mode;
				
				 Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
				 Mode.SetHideCursorDuringCapture(false);
				 WidgetInstance->AddToViewport(9999); // Z-order, this just makes it render on the very top.
				 
			}
		}
	}*/
}

void ATownDefaultPawn::EnableInputOnFinish()
{
	bEnableInput = true;
}

void ATownDefaultPawn::SetMappedWidgetInstance(FString Key, UTownBuildingWidgetBase* Widget)
{
	FBuildingData* Data = BuildingDataMap.Find(Key);
    UE_LOG(LogTemp, Warning, TEXT("TABLE , %s"),*(Key));

    if(Data)
    {
       Data->UiWidgetInstance = Widget;
        if(Data->UiWidgetInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT(" SAVED "));
        }
    	UE_LOG(LogTemp, Warning, TEXT("NOT SAVED 1 "));
    }else
    {
        UE_LOG(LogTemp, Warning, TEXT("NOT SAVED 2 "));
    }
}

UTownBuildingWidgetBase* ATownDefaultPawn::GetMappedWidget(FString Key)
{
	FBuildingData* Data = BuildingDataMap.Find(Key);
            UE_LOG(LogTemp, Warning, TEXT("GetMappedWidget  , %s"),*(Key));

    if(Data)
    {
        if(Data->UiWidget)
            UE_LOG(LogTemp, Warning, TEXT("Get is not null"  ));

        return  Data->UiWidget;

    }

    return  nullptr;
}

UTownBuildingWidgetBase* ATownDefaultPawn::GetMappedWidgetInstance(FString Key)
{
	FBuildingData* Data = BuildingDataMap.Find(Key);
    if(Data)
    {
        return  Data->UiWidgetInstance;
    }

    return  nullptr;
}

ULevelSequence* ATownDefaultPawn::GetMappedSequenceAsset(FString Key)
{
	 FBuildingData* Data = BuildingDataMap.Find(Key);
    if(Data)
    {
        return  Data->SequenceAsset;
    }

    return  nullptr;
}

// Called when the game starts or when spawned
void ATownDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATownDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult Hit;
	ATownPlayerController* PlayerController = Cast<ATownPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, Hit);
	if (Hit.bBlockingHit)
	{
		if(Hit.Actor != nullptr)
		{
			UActorComponent* Building = Hit.Actor->GetComponentByClass(UTownBuildingActorComponent::StaticClass());
			if(Building != nullptr)
			{
				UActorComponent* StaticMesh = Hit.Actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
				if(StaticMesh)
				{
					UStaticMeshComponent* NewStaticMesh = Cast<UStaticMeshComponent>(StaticMesh);
					HoveredBuildingStaticMeshComp = NewStaticMesh;
					HoveredBuildingStaticMeshComp->SetRenderCustomDepth(true);
					return;
				}
			}
		}
	}

	
	if(HoveredBuildingStaticMeshComp)
	{
		HoveredBuildingStaticMeshComp->SetRenderCustomDepth(false);
		HoveredBuildingStaticMeshComp = nullptr;
	}

}

// Called to bind functionality to input
void ATownDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent)
	{
		return;
	}
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
	PlayerInputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &ATownDefaultPawn::LeftClickHandler);
	//PlayerInputComponent->BindAction("LeftMouseClick", IE_Released, this, &AMyDefaultPawn::LeftClickReleaseHandler);
	PlayerInputComponent->BindAction("RightMouseClick", IE_Pressed, this, &ATownDefaultPawn::RightClickHandler);
	// PlayerInputComponent->BindAction("RightMouseClick", IE_Released, this, &AMyDefaultPawn::RightClickReleaseHandler);

}

