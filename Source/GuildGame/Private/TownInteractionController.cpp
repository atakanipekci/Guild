// Fill out your copyright notice in the Description page of Project Settings.


#include "TownInteractionController.h"



#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "TownBuildingActorComponent.h"
#include "TownBuildingWidgetBase.h"
#include "TownGameModeBase.h"
#include "TownPlayerController.h"
#include "TownYesOrNoWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"

UTownInteractionController::UTownInteractionController()
{
	
}

UTownInteractionController::~UTownInteractionController()
{
	
}

void UTownInteractionController::ManuelConstructor(EInteractionStatee NewState, ATownPlayerController* NewPlayerController)
{
	this->State = State;
	this->PrevState = State;
    this->PlayerController = NewPlayerController;
}

void UTownInteractionController::Tick()
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

void UTownInteractionController::RaycastUnderCursor()
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
			UActorComponent* Building = Hit.Actor->GetComponentByClass(UTownBuildingActorComponent::StaticClass());

			
			UActorComponent* StaticMesh = Hit.Actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
			SetRenderCustomDepth(StaticMesh, true);

			if(Building)
			{
				HoveredBuilding = Cast<UTownBuildingActorComponent>(Building);
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

void UTownInteractionController::PlaySequence()
{
	if(SequenceAsset == nullptr || PlayerController == nullptr)
		return;
	
	if (SequencePlayer == nullptr && SequenceActor == nullptr)
	{
          SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(PlayerController->GetWorld(), SequenceAsset, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	}
     
    if (SequencePlayer)
    {
    	//UE_LOG(LogTemp, Warning, TEXT("SETTING SEQ , %s"),*(SequenceAsset->GetName()));
    	SequencePlayer->Stop();
    	// SequencePlayer->OnFinished.RemoveAll(this);
    	SequenceActor->SetSequence(SequenceAsset);
    	//SequencePlayer->OnFinished.AddUniqueDynamic(this, &UTownInteractionController::OnSequenceFinish);
        SequencePlayer->Play();
    }
}

void UTownInteractionController::PlaySequenceReverse()
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



void UTownInteractionController::SetRenderCustomDepth(UActorComponent* Building, bool EnableDepth)
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

void UTownInteractionController::LeftClickHandler()
{
	if(State == EInteractionStatee::BuildingSelection)
	{
		ZoomInToBuilding();
	}
}

void UTownInteractionController::ZoomInToBuilding()
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
		//UE_LOG(LogTemp, Warning, TEXT("RAYCAST ITEM, %s"),*(Hit.Actor->GetName()));
}


void UTownInteractionController::ZoomOutFromBuilding()
{
	if(bEnableInput == false)
		return;

	if(SelectedBuilding && State == EInteractionStatee::BuildingWidget  && SequenceAsset != nullptr)
	{
		ChangeState(State, EInteractionStatee::BuildingSelection);

		 bEnableInput = false;

		
		if(UTownYesOrNoWidget::YesOrNoWidgetInstance)
			UTownYesOrNoWidget::YesOrNoWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);

	
		if(SelectedBuilding->GetConstructionState() == EBuildingConstructionState::Preview)
		{
		    SelectedBuilding->SetConstructionState(EBuildingConstructionState::NotConstructed);
		}
		PlaySequenceReverse();
	}
}

void UTownInteractionController::ToggleWidget()
{
	if(SelectedBuilding)
	{
		UUserWidget* WidgetInstance = WidgetManager::GetWidgetInstance(SelectedBuilding->WidgetKey);

		if(WidgetInstance != nullptr)
		{
			UTownBuildingWidgetBase* BuildingWidgetInstance = Cast<UTownBuildingWidgetBase>(WidgetInstance);
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
					BuildingWidgetInstance->OnEnabled();
					BuildingWidgetInstance->SetVisibility(ESlateVisibility::Visible);
					BuildingWidgetInstance->PlayAnimation(BuildingWidgetInstance->OpenUpAnimation);
				}
			}
		}
		else
		{
			UTownBuildingWidgetBase* NewWidget = CreateWidget<UTownBuildingWidgetBase>(PlayerController->GetWorld(), WidgetManager::GetWidget(SelectedBuilding->WidgetKey));
			WidgetManager::SetWidgetInstance(SelectedBuilding->WidgetKey, NewWidget);

			ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(PlayerController));
			if(GameMode->MenuUiContainerOverlay)
			{
				GameMode->MenuUiContainerOverlay->AddChildToOverlay(NewWidget);
			}

			bIsSequenceReversed = true;
			FWidgetAnimationDynamicEvent OnFinishEvent;
			OnFinishEvent.BindDynamic(this, &UTownInteractionController::OnWidgetAnimationFinish);
			NewWidget->OnEnabled();
			NewWidget->BindToAnimationFinished(NewWidget->OpenUpAnimation, OnFinishEvent);
			NewWidget->PlayAnimation(NewWidget->OpenUpAnimation);
			
			NewWidget->BindToAnimationFinished(NewWidget->CloseDownAnimation, OnFinishEvent);
		}
	}
}

void UTownInteractionController::OnSequenceFinish()
{
}

void UTownInteractionController::OnWidgetAnimationFinish()
{
		UE_LOG(LogTemp, Warning, TEXT("ONANIMFINISH"));
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

void UTownInteractionController::ChangeState(EInteractionStatee From, EInteractionStatee To)
{
	if(State != To)
	{
		PrevState = State;
	}
	
	State = To;
	OnStateChange(From, true);
	OnStateChange(To, false);
}

void UTownInteractionController::OnStateChange(const EInteractionStatee NewState, const bool IsFrom)
{
	if(NewState == EInteractionStatee::BuildingSelection)
	{
		if(IsFrom)
		{
			if(HoveredBuilding)
			{
				UActorComponent* StaticMesh = HoveredBuilding->GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass());
				if(StaticMesh)
					SetRenderCustomDepth(StaticMesh, false);
				HoveredBuilding = nullptr;
			}
		}
		else
		{
			
		}
	}
	else if(NewState == EInteractionStatee::BuildingWidget)
	{
		if(IsFrom)
		{
		}
		else
		{
			
		}
	}
	else if(NewState == EInteractionStatee::MenuWidget)
	{
		if(IsFrom)
		{
		}
		else
		{
			
		}
	}
}

void UTownInteractionController::ToPrevStateState()
{
	ChangeState(State, PrevState);
}

