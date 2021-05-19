// Fill out your copyright notice in the Description page of Project Settings.


#include "TownBuildingInteractionManager.h"



#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "BuildingActorComponent.h"
#include "GuildGame/Widgets/BuildingWidgetBase.h"
#include "TownGameModeBase.h"
#include "TownPlayerController.h"
#include "GuildGame/Widgets/YesOrNoWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"

UTownBuildingInteractionManager::UTownBuildingInteractionManager()
{
	
}

UTownBuildingInteractionManager::~UTownBuildingInteractionManager()
{
	
}

void UTownBuildingInteractionManager::ManuelConstructor(EInteractionStatee NewState, ATownPlayerController* NewPlayerController)
{
	this->State = State;
	this->PrevState = State;
    this->PlayerController = NewPlayerController;
}

void UTownBuildingInteractionManager::Tick()
{
    if(State == EInteractionStatee::BuildingSelection)
    {
	    RaycastUnderCursor();
    }

	// if(SelectedBuilding == nullptr)
	// 	UE_LOG(LogTemp, Warning, TEXT("TICK SELECTED IS NULL"));
	//
	// if(HoveredBuilding == nullptr)
	// 	UE_LOG(LogTemp, Warning, TEXT("Hovered IS NULL"));
	//
	// UE_LOG(LogTemp, Warning, TEXT("BUILD STATE %d"), State);
	//
	// 	UE_LOG(LogTemp, Warning, TEXT("ENABLE INPUT %d"), bEnableInput);


}

void UTownBuildingInteractionManager::RaycastUnderCursor()
{
    if(PlayerController == nullptr)
		return;


	FHitResult Hit;
	PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, Hit);
	
	if (Hit.bBlockingHit)
	{
		if(Hit.Actor != nullptr)
		{
			if(HoveredBuilding)
			{
				//Reset previous Building's highlight
				UActorComponent* StaticMesh = HoveredBuilding->GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());
				SetRenderCustomDepth(StaticMesh, false);
			}
			UActorComponent* Building = Hit.Actor->GetComponentByClass(UBuildingActorComponent::StaticClass());

			
			UActorComponent* StaticMesh = Hit.Actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
			SetRenderCustomDepth(StaticMesh, true);

			if(Building)
			{
				HoveredBuilding = Cast<UBuildingActorComponent>(Building);
				return;
			}
		}
	}

	if(HoveredBuilding)
	{
		UActorComponent* StaticMesh = HoveredBuilding->GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());
		if(StaticMesh)
			SetRenderCustomDepth(StaticMesh, false);
	}

	HoveredBuilding = nullptr;
}

void UTownBuildingInteractionManager::PlaySequence()
{
		UE_LOG(LogTemp, Warning, TEXT("PlaySequence"));
	if(SequenceAsset == nullptr || PlayerController == nullptr)
		return;
	
	if (SequencePlayer == nullptr && SequenceActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Sequence Player"));
          SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(PlayerController->GetWorld(), SequenceAsset, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	}
     
    if (SequencePlayer)
    {
    	UE_LOG(LogTemp, Warning, TEXT("Sequence PLAY"));
    	//UE_LOG(LogTemp, Warning, TEXT("SETTING SEQ , %s"),*(SequenceAsset->GetName()));
    	SequencePlayer->Stop();
    	// SequencePlayer->OnFinished.RemoveAll(this);
    	SequenceActor->SetSequence(SequenceAsset);
    	//SequencePlayer->OnFinished.AddUniqueDynamic(this, &UTownInteractionController::OnSequenceFinish);
        SequencePlayer->Play();
    }
}

void UTownBuildingInteractionManager::PlaySequenceReverse()
{
	if (SequenceAsset && SequencePlayer && SequenceActor)
	{
	   SequencePlayer->Stop();
	   //SequencePlayer->OnFinished.RemoveAll(this);
	   SequenceActor->SetSequence(SequenceAsset);
	  // SequencePlayer->OnFinished.AddUniqueDynamic(this, &UTownInteractionController::OnSequenceFinish);
       SequencePlayer->PlayReverse();
	}
}



void UTownBuildingInteractionManager::SetRenderCustomDepth(UActorComponent* Building, bool EnableDepth)
{
	if(Building != nullptr)
	{
		UStaticMeshComponent* NewStaticMesh = Cast<UStaticMeshComponent>(Building);
				
		if(NewStaticMesh != nullptr)
		{
			NewStaticMesh->SetRenderCustomDepth(EnableDepth);
		}
	}
}

void UTownBuildingInteractionManager::LeftClickHandler()
{
	 UE_LOG(LogTemp, Warning, TEXT("PLAYER CONTROLLER 1"));

	if(State == EInteractionStatee::BuildingSelection)
	{
		ZoomInToBuilding();
	}
}

void UTownBuildingInteractionManager::ZoomInToBuilding()
{
	if(bEnableInput == false)
		return;

	SelectedBuilding = HoveredBuilding;

	if(SelectedBuilding)
	{
		bEnableInput = false;

		SequenceAsset = WidgetManager::GetSequence(SelectedBuilding->SequenceKey);

		ChangeState(State, EInteractionStatee::BuildingWidget);

		PlaySequence();

	}
}


void UTownBuildingInteractionManager::ZoomOutFromBuilding()
{
	if(bEnableInput == false)
		return;

	if(SelectedBuilding && State == EInteractionStatee::BuildingWidget  && SequenceAsset != nullptr)
	{
		ChangeState(State, EInteractionStatee::BuildingSelection);

		 bEnableInput = false;

		
		if(UYesOrNoWidget::YesOrNoWidgetInstance)
			UYesOrNoWidget::YesOrNoWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);

	
		if(SelectedBuilding->GetConstructionState() == EBuildingConstructionState::Preview)
		{
		    SelectedBuilding->SetConstructionState(EBuildingConstructionState::NotConstructed);
		}
		PlaySequenceReverse();
	}
}

void UTownBuildingInteractionManager::ToggleWidget()
{
	if(SelectedBuilding)
	{
		UUserWidget* WidgetInstance = WidgetManager::GetWidgetInstance(SelectedBuilding->WidgetKey);

		if(WidgetInstance != nullptr)
		{

			UBuildingWidgetBase* BuildingWidgetInstance = Cast<UBuildingWidgetBase>(WidgetInstance);
			if(BuildingWidgetInstance)
			{
				BuildingWidgetInstance->StopAllAnimations();

				 UPanelWidget* Parent = BuildingWidgetInstance->GetParent();
				// BuildingWidgetInstance->RemoveFromParent();
				//
				// Parent->AddChild(BuildingWidgetInstance);


				if(bIsSequenceReversed == true)
				{
					bIsSequenceReversed = false;

					
					BuildingWidgetInstance->PlayAnimation(BuildingWidgetInstance->CloseDownAnimation);
				}
				else
				{
					bIsSequenceReversed = true;
					BuildingWidgetInstance->Refresh();
					BuildingWidgetInstance->SetVisibility(ESlateVisibility::Visible);
					BuildingWidgetInstance->PlayAnimation(BuildingWidgetInstance->OpenUpAnimation);
				}
			}
		}
		else
		{
			UBuildingWidgetBase* NewWidget = CreateWidget<UBuildingWidgetBase>(PlayerController->GetWorld(), WidgetManager::GetWidget(SelectedBuilding->WidgetKey));
			WidgetManager::SetWidgetInstance(SelectedBuilding->WidgetKey, NewWidget);
			
			ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(PlayerController));
			if(GameMode->HudBuildingWidgetsContainerOverlay)
			{
				GameMode->HudBuildingWidgetsContainerOverlay->AddChildToOverlay(NewWidget);
			}

			bIsSequenceReversed = true;
			OnFinishEvent.Clear();
			OnFinishEvent.BindDynamic(this, &UTownBuildingInteractionManager::OnWidgetAnimationFinish);
			NewWidget->Refresh();
			NewWidget->BindToAnimationFinished(NewWidget->OpenUpAnimation, OnFinishEvent);
			NewWidget->PlayAnimation(NewWidget->OpenUpAnimation);
			
			NewWidget->BindToAnimationFinished(NewWidget->CloseDownAnimation, OnFinishEvent);
		}
	}
}

void UTownBuildingInteractionManager::OnSequenceFinish()
{
}

void UTownBuildingInteractionManager::OnWidgetAnimationFinish()
{

	if(SelectedBuilding && State == EInteractionStatee::BuildingSelection)
	{
		UUserWidget* BuildingWidgetInstance = WidgetManager::GetWidgetInstance(SelectedBuilding->WidgetKey);
		if(BuildingWidgetInstance)
		{
			BuildingWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		SelectedBuilding = nullptr;
	}


	bEnableInput = true;
}

void UTownBuildingInteractionManager::ChangeState(EInteractionStatee From, EInteractionStatee To)
{
	if(State != To)
	{
		PrevState = State;
	}
	
	State = To;
	OnStateChange(To, From);
}

void UTownBuildingInteractionManager::OnStateChange(const EInteractionStatee NewState, const EInteractionStatee OldState)
{
	if(OldState == EInteractionStatee::BuildingSelection)
	{
		if(HoveredBuilding)
		{
			UActorComponent* StaticMesh = HoveredBuilding->GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());
			if(StaticMesh)
				SetRenderCustomDepth(StaticMesh, false);
			HoveredBuilding = nullptr;
		}
	}
	
}

void UTownBuildingInteractionManager::ToPrevStateState()
{
	ChangeState(State, PrevState);
}

