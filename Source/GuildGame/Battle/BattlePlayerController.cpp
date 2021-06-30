// Fill out your copyright notice in the Description page of Project Settings.


#include "BattlePlayerController.h"

#include "AIController.h"
#include "BattleCameraSpectatorPawn.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "BattleControllerState.h"
#include "GuildGame/GridSystem/GridFloor.h"
#include "GGLogHelper.h"
#include "GuildGame/GuildGameGameModeBase.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "GuildGame/Widgets/BattleHudWidget.h"
#include "Kismet/GameplayStatics.h"

ABattlePlayerController::ABattlePlayerController()
	:Super()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	PrimaryActorTick.bCanEverTick = true;
	States.Add(new ControllerStatePlacement(this));
	States.Add(new ControllerStateDefault(this));
	States.Add(new ControllerStateCastingSkill(this));
	if(States.Num() != 0)
	{
		ActiveState = States[0];
		StateIndex = 0;
	}
}

ABattlePlayerController::~ABattlePlayerController()
{
	for(int i = 0; i < States.Num(); i++)
	{
		delete States[i];
		States.RemoveAt(i);
	}
}


void ABattlePlayerController::BeginPlay()
{

}

void ABattlePlayerController::Tick(float DeltaTime)
{
	if(ActiveState)
	{
		ActiveState->Update();
	}
}

BattleControllerState* ABattlePlayerController::GetActiveState() const
{
	return ActiveState;
}

void ABattlePlayerController::SetState(int AStateIndex)
{
	if(AStateIndex >= States.Num())
	{
		return;
	}
	StateIndex = AStateIndex;

	ActiveState = States[AStateIndex];
}

void ABattlePlayerController::ChangeStateTo(EControllerStateIndex AStateIndex)
{
	int Index = static_cast<int>(AStateIndex);
	if(Index >= States.Num())
	{
		return;
	}

	if(States[Index])
	{
		if(!States[Index]->CanChangeTo())
		{
			return;
		}
	}
	
	if(ActiveState)
	{
		ActiveState->ChangeFrom();
	}
	StateIndex = Index;
	ActiveState = States[Index];
	ActiveState->ChangeTo();

	if(SelectedCharacter)
	{
		SelectedCharacter->CallRefreshHudDelegate();
	}
}

bool ABattlePlayerController::UpdateSelectedGrid(bool DrawPathTo)
{
	if(GridFloor == nullptr)
	{
		return false;
	}
	
	GridManager* GridMan =  GridFloor->GetGridManager();
	if(GridMan == nullptr)
	{
		return false;
	}
	
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);

	if(TraceResult.GetActor() == nullptr)
	{
		GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(0), false);
		return false;
	}

	int GridIndex = GridMan->WorldToGrid(TraceResult.ImpactPoint);

	int MaxIndex = GridFloor->ColumnCount*GridFloor->RowCount -1;
	if(GridIndex < 0 || GridIndex > MaxIndex)
	{
		GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(0), false);
		return false;
	}
	
	if(GridIndex!=SelectedGridIndex || (SelectedCharacter != nullptr && SelectedCharacter->GetSize() == ECharacterSize::Large))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT(" %d"), GridIndex));
		SelectedGridIndex = GridIndex;
		if(SelectedCharacter)
		{
			if(SelectedCharacter->GetSize() == ECharacterSize::Large && StateIndex == static_cast<int>(EControllerStateIndex::Movement))
			{
				FVector NewPos = GridMan->GetClosestLineIntersection(TraceResult.ImpactPoint) -
					FVector(GridMan->GetGridSize(), GridMan->GetGridSize(), 0);
				GridFloor->UpdateSelectedGrid(NewPos, true, 2);
				
			}
			else
			{
				GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(GridIndex), true);
			}
		}
		else
		{
			GridFloor->UpdateSelectedGrid(GridMan->GetGridBottomLeft(GridIndex), true);
		}
		if(SelectedCharacter && SelectedCharacter->GetStatus() == ECharacterStatus::Idle && DrawPathTo && SelectedCharacter->GetCurrentAP() > 0)
		{
			int start = GridMan->WorldToGrid(SelectedCharacter->GetActorLocation());
			int end = SelectedGridIndex;
			if(SelectedCharacter->GetSize() == ECharacterSize::Large)
			{
				end = GridMan->WorldToGrid(GridMan->GetClosestLineIntersection(TraceResult.ImpactPoint));
				GridFloor->DrawPath(start,end, true);
			}
			else
			{
				GridFloor->DrawPath(start,end);
			}

			float Dist = GridFloor->GetPathLength(GridMan->WorldToGrid(SelectedCharacter->GetActorLocation()), SelectedGridIndex);

			int ApCost = SelectedCharacter->GetApCostByDistance(Dist);

			UE_LOG(LogTemp, Warning, TEXT("AP COST At Location %d"), ApCost);
		}

		if(SelectedCharacter)
		{
			TArray<Grid*> CharacterGrids;
			GridMan->GetLargeCharacterGrids(SelectedCharacter->GetCurrentIndex(), CharacterGrids);

			if(GridMan->DoesInclude(SelectedCharacter->GetMovableGrids(), SelectedGridIndex)
				|| (SelectedCharacter->GetSize() == ECharacterSize::Large &&
					GridMan->DoesInclude(&CharacterGrids, SelectedGridIndex)))
			{
				if(SelectedCharacter->GetSize() == ECharacterSize::Large)
				{
					FVector NewPos = GridMan->GetClosestLineIntersection(TraceResult.ImpactPoint);
					int Index = GridMan->WorldToGrid(NewPos);
					TArray<Grid*> Neighbours;
					
					GridMan->GetLargeCharacterGrids(Index, Neighbours);
					bool Flag = true;
					for (auto Element : Neighbours)
					{
						if(Element == nullptr)
						{
							Flag = false;
							break;
						}
						if(!(GridMan->DoesInclude(SelectedCharacter->GetMovableGrids(), Element->Index)))
						{
							if(!(GridMan->DoesInclude(&CharacterGrids, Element->Index)))
							{
								Flag = false;
								break;
							}
						}
					}

					if(Flag)
					{
						GridFloor->SetSelectedGridColorType(EISMType::Movement);
					}
					else
					{
						GridFloor->SetSelectedGridColorType(EISMType::Empty);
					}				
				}
				else
				{
					GridFloor->SetSelectedGridColorType(EISMType::Movement);
				}
			}
			else
			{
				GridFloor->SetSelectedGridColorType(EISMType::Empty);
			}
		}
		
		return true;
	}
	return false;
}

void ABattlePlayerController::DrawPath(int StartIndex, int EndIndex, bool LargeCharacter) const
{
	if(StartIndex == EndIndex || GridFloor == nullptr)
	{
		return;
	}

	GridFloor->DrawPath(StartIndex,EndIndex, LargeCharacter);
}

void ABattlePlayerController::SetGridFloor(AGridFloor* Grid)
{
	GridFloor = Grid;
}

AGridFloor* ABattlePlayerController::GetGridFloor() const
{
	return GridFloor;
}

void ABattlePlayerController::SelectCharAtMousePos()
{
	AGGCharacter* Char = GetCharacterFromMousePos();
	if(Char && Char->GetStatus() != ECharacterStatus::Dead)
	{
		SetSelectedCharacter(Char);
	}
}

void ABattlePlayerController::MoveSelectedChar()
{
	if(SelectedCharacter == nullptr || SelectedGridIndex < 0 || GridFloor == nullptr)
	{
		return;
	}
	GridManager* GridMan =  GridFloor->GetGridManager();
	if(GridMan == nullptr)
	{
		return;
	}

	float Dist = 0;
	if(SelectedCharacter->GetSize() == ECharacterSize::Normal)
	{
		Dist = GridFloor->GetPathLength(GridMan->WorldToGrid(SelectedCharacter->GetActorLocation()), SelectedGridIndex);
		if(Dist <= 0 || Dist > SelectedCharacter->GetDefaultMovementRange())
		{
			return;
		}
		FVector Target = GridMan->GetGridCenter(SelectedGridIndex);
		Target.Z = SelectedCharacter->GetActorLocation().Z;
		SelectedCharacter->MoveTo(Target);
	}
	else if(SelectedCharacter->GetSize() == ECharacterSize::Large)
	{
		FHitResult TraceResult(ForceInit);
		this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceResult);
		if(TraceResult.IsValidBlockingHit())
		{
			
		}
		FVector NewPos = GridMan->GetClosestLineIntersection(TraceResult.ImpactPoint);
		int Index = GridMan->WorldToGrid(NewPos);
		Dist = GridFloor->GetPathLength(GridMan->WorldToGrid(SelectedCharacter->GetActorLocation()), Index);
		if(Dist <= 0 || Dist > SelectedCharacter->GetDefaultMovementRange())
		{
			return;
		}
		FVector Target = GridMan->GetGridBottomLeft(Index);
		Target.Z = SelectedCharacter->GetActorLocation().Z;
		SelectedCharacter->MoveTo(Target);
	}
	
	int ApCost = SelectedCharacter->GetApCostByDistance(Dist);
	SelectedCharacter->TryToSpendAP(ApCost);

	AGuildGameGameModeBase* GameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameMode)
	{
		ABattleCameraSpectatorPawn* Spectator = GameMode->CameraSpectatorPawn;
		if(Spectator)
		{
			Spectator->LerpCameraToCharacterAndFollow(SelectedCharacter, 1);
		}

		if(GameMode->HudWidgetInstance)
		{
			GameMode->HudWidgetInstance->SetSkillsPanelHidden();
		}
	}
}

void ABattlePlayerController::SetSelectedCharacter(AGGCharacter* NewCharacter)
{
	if(SelectedCharacter)
	{
		SelectedCharacter->Deselect();
	}
	SelectedCharacter = NewCharacter;
	if(SelectedCharacter)
	{
		SelectedCharacter->SetSelected();
	}

	if(NewCharacter)
	{
		AGuildGameGameModeBase* BattleGameMode = Cast<AGuildGameGameModeBase>(UGameplayStatics::GetGameMode(NewCharacter));
		if(BattleGameMode)
		{
			if(BattleGameMode->HudWidgetInstance)
			{
				BattleGameMode->HudWidgetInstance->RefreshSkillsArray(SelectedCharacter);
			}

			ABattleCameraSpectatorPawn* Spectator = BattleGameMode->CameraSpectatorPawn;
			if(Spectator)
			{
				if(BattleGameMode->HudWidgetInstance)
				{
					BattleGameMode->HudWidgetInstance->SetSkillsPanelHidden();
					
					TArray<FTimedEvent> OnCompleteDelegates;
					FTimedEvent OnDelayedComplete;
					
					float DelayTime = 0;
					if(NewCharacter->IsStunned())
					{
						DelayTime = 1.4f;
						OnDelayedComplete.BindDynamic(BattleGameMode, &AGuildGameGameModeBase::Next);
					}
					else
					{
						FTimedEvent OnCompleteDelegate1;
						OnCompleteDelegate1.BindDynamic(BattleGameMode->HudWidgetInstance, &UBattleHudWidget::SetSkillsPanelVisible);
						OnCompleteDelegates.Add(OnCompleteDelegate1);
					}
					FTimedEvent OnCompleteDelegate2;
					OnCompleteDelegate2.BindDynamic(NewCharacter, &AGGCharacter::OnTurnBegins);
					OnCompleteDelegates.Add(OnCompleteDelegate2);

					Spectator->LerpCameraToCharacter(NewCharacter, 0.8f, DelayTime,false, OnCompleteDelegates,OnDelayedComplete);
				}
			}
		}
	}
}

AGGCharacter* ABattlePlayerController::GetSelectedCharacter() const
{
	return SelectedCharacter;
}

AGGCharacter* ABattlePlayerController::GetCharacterFromMousePos() const
{
	FHitResult TraceResult(ForceInit);
	this->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, TraceResult);
	if (TraceResult.GetActor() != nullptr)
	{	
		AGGCharacter* HitCharacter = Cast<AGGCharacter>(TraceResult.GetActor());			
		if(HitCharacter != nullptr)
		{
			return HitCharacter;
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}
