// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnInfoWidget.h"

#include "TurnInfoNodeWidget.h"
#include "Algo/StableSort.h"
#include "Blueprint/UserWidgetPool.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GuildGame/Characters/GGCharacter.h"
#include "GuildGame/Managers/ImageManager.h"
#include "GuildGame/Managers/TimedEventManager.h"
#include "GuildGame/Managers/WidgetManager.h"

void UTurnInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Pool.SetWorld(GetWorld());
}

void UTurnInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(Pool.IsInitialized())
	{
		Pool.ReleaseAllSlateResources();
	}
}

UTurnInfoNodeWidget* UTurnInfoWidget::AddNode(bool IsRoundSeperator, class AGGCharacter* Char)
{
	UTurnInfoNodeWidget* Node = CreateEmptyNode();

	if(Node && TurnGridPanel)
	{
		if(GridIndexHead - 1 >= 0)
		{
			UTurnInfoNodeWidget* LeftNode = Cast<UTurnInfoNodeWidget>(TurnGridPanel->GetChildAt(GridIndexHead - 1));
			//Even though we swap nodes and the child of grid with the index of (GridIndexHead - 1) may no longer be at the and however the last
			//node is always the seperator thus it wont be swapped, so the new nodes will be at the correct position
			if(LeftNode)
			{
				Node->LeftNode = LeftNode;
				LeftNode->RightNode = Node;
				FVector2D Translation = FVector2D::ZeroVector;
				Translation.X = LeftNode->RenderTransform.Translation.X;
				Node->SetRenderTranslation(Translation);
				ATimedEventManager::CloneWidgetMovementAsync(Node->LeftNode, Node, "SlideLeft");
			}
		}
		if(Node)
		{
			if(DefaultYSize > 0)
			{
				float Xratio = Xratio = (DefaultXSize/DefaultYSize);
				if(IsRoundSeperator == false)
				{
					Node->Character = Char;
					if(Char && Node->Portrait)
					{
						Node->Portrait->SetVisibility(ESlateVisibility::Visible);
						ImageManager::SetPortraitTextureByClass(Char->GetCharacterClass(), Node->Portrait);
					}
					if(Node->BackGround)
					{
						Node->BackGround->SetVisibility(ESlateVisibility::Visible);
					}
					if(Node->SeperatorBg)
					{
						Node->SeperatorBg->SetVisibility(ESlateVisibility::Collapsed);
					}
					if(Node->Text)
					{
						Node->Text->SetVisibility(ESlateVisibility::Collapsed);
					}

					if(Node->NodePanel)
					{
						if(Node->NodePanel->Slot)
						{
							UCanvasPanelSlot* SlotData = Cast<UCanvasPanelSlot>(Node->NodePanel->Slot);

							if(SlotData)
							{
								if(GridIndexHead == 0)
								{
									SlotData->SetSize(FVector2D(DefaultXSize * Xratio, BigYSize));
								}
								else
								{
									SlotData->SetSize(FVector2D(DefaultXSize, DefaultYSize));
								}
							}
						}
					}
				}
				else
				{
					if(Node->Portrait)
					{
						Node->Portrait->SetVisibility(ESlateVisibility::Collapsed);
					}
					if(Node->BackGround)
					{
						Node->BackGround->SetVisibility(ESlateVisibility::Collapsed);
					}
					if(Node->SeperatorBg)
					{
						Node->SeperatorBg->SetVisibility(ESlateVisibility::Visible);
					}
					if(Node->Text)
					{
						Node->Text->SetVisibility(ESlateVisibility::Visible);
					}
	
					if(Node->NodePanel)
					{
						if(Node->NodePanel->Slot)
						{
							UCanvasPanelSlot* SlotData = Cast<UCanvasPanelSlot>(Node->NodePanel->Slot);

							if(SlotData)
							{
								SlotData->SetSize(FVector2D((DefaultXSize/4), DefaultYSize));
							}
						}
					}
				}
			}
		}

		if(Node)
		{
			Node->IsSeperator = IsRoundSeperator;
			Node->Index = ActiveNodeIndex;
			Node->GridIndex = GridIndexHead;
			TurnGridPanel->AddChildToGrid(Node, 0, GridIndexHead);

			if(Node->AddAnimation)
				Node->PlayAnimation(Node->AddAnimation);

			GridIndexHead++;
			ActiveNodeIndex++;
		}
	}
	return Node;
}

UTurnInfoNodeWidget* UTurnInfoWidget::CreateEmptyNode()
{
		if(Pool.IsInitialized())
		{
			UTurnInfoNodeWidget* PoolObject = Cast<UTurnInfoNodeWidget>(Pool.GetOrCreateInstance(WidgetManager::GetWidget(EWidgetKeys::TurnInfoNode)));
			// if(PoolObject)
			// {
			// 	PoolObject->EmptyData();
			// }
			return PoolObject;
		}
		else
		{
			return Cast<UTurnInfoNodeWidget>(WidgetManager::CreateWidgetInstance(EWidgetKeys::TurnInfoNode, GetWorld()));
		}
}

void UTurnInfoWidget::FillGapWithRounds()
{
	int CurrentAmount = 0;

	for (int i = 0; i < TurnNodes.Num(); ++i)
	{
		CurrentAmount += TurnNodes[i].Num();
	}

	int AmountToAdd = FMath::Clamp(20 - CurrentAmount, 0, 20);
	int RowLength = CharactersList.Num();

	if(RowLength > 0)
	{
		for (int i = 0; i < AmountToAdd / RowLength; ++i)
		{
			AddNewRound();
		}
	}
}

void UTurnInfoWidget::RemoveRandomNode(bool Slide)
{
	if(TurnGridPanel)
	{
		//AddNewRound();
		// int RandIndex = FMath::RandRange(0, Tur.Num() - 1);
		//
		// if(RandIndex < AvailableNodes.Num())
		// {
		// 	UTurnInfoNodeWidget* Node =AvailableNodes[RandIndex];
		// 	RemoveNodeWidget(Node, Slide, RandIndex == 0 ? true : false);
		// }

		// RemoveNodeWidget(TurnNodes[1][0], Slide, false);
		//RemoveDeadCharacter(CharactersList[1]);

		//AddNode(false, CharactersList[1]);
	}
}

void UTurnInfoWidget::RemoveAtTop(bool Slide)
{
	if(TurnGridPanel)
	{
		if(TurnNodes.Num() > 0 && TurnNodes[0].Num() > 0)
		{
			RemoveNodeWidget(TurnNodes[0][0], Slide, true);
		}
	}
}

void UTurnInfoWidget::RemoveNodeWidget(UTurnInfoNodeWidget* Node, bool Slide, bool IsFirst)
{
	if(Node)
	{
		Node->IsRemoved = true;
		Node->Index = -1;
		float RemoveTime = 0.5f;
		// ATimedEventManager::LerpWidgetOpacity(Node, RemoveTime,Node->RenderOpacity, 0, false,GetWorld());
		// ATimedEventManager::MoveWidgetAsync(Node, RemoveTime, FVector2D(0, 50),  GetWorld());
		//TODO:We can turn this lerp into animation

		if(Node->RemoveAnimation)
			Node->PlayAnimation(Node->RemoveAnimation);
		

		
		float Width = Node->GetDesiredSize().X;

		if(Node->RightNode)
		{
			if(Node->LeftNode)
			{
				Node->RightNode->LeftNode = Node->LeftNode;
				Node->LeftNode->RightNode = Node->RightNode;
			}
			else
			{
				Node->RightNode->LeftNode = nullptr;
			}
		}
		else
		{
			if(Node->LeftNode)
			{
				Node->LeftNode->RightNode = nullptr;
			}
		}
		RemoveNodeFromList(Node);
		
		if(Node && Node->RightNode && Slide)
		{
			RecursiveSlideAll(Node->RightNode, Width, 0, IsFirst);
		}
	}
}

void UTurnInfoWidget::RemoveNodeFromList(int i, int j)
{
	if(i < TurnNodes.Num() && j < TurnNodes[i].Num())
	{
		TurnNodes[i].RemoveAt(j);
		if(TurnNodes[i].Num() <= 0)
		{
			TurnNodes.RemoveAt(i);
			// AddNewRound();
			FillGapWithRounds();
		}
	}
}

void UTurnInfoWidget::RemoveNodeFromList(UTurnInfoNodeWidget* Node)
{
	if(Node)
	{
		for (int i = 0; i < TurnNodes.Num(); ++i)
		{
			for (int j = 0; j < TurnNodes[i].Num(); ++j)
			{
				if((TurnNodes[i])[j] == Node)
				{
					TurnNodes[i].RemoveAt(j);
					if(TurnNodes[i].Num() <= 0)
					{
						TurnNodes.RemoveAt(i);
						// AddNewRound();
						FillGapWithRounds();
					}

					return;
				}
			}
		}
	}
}

void UTurnInfoWidget::AddNewRound()
{
	TArray<UTurnInfoNodeWidget*> NewRound;
	for (int x = 0; x < CharactersList.Num(); ++x)
	{
		if(CharactersList[x] && CharactersList[x]->GetStatus() != ECharacterStatus::Dead)
		{
			UTurnInfoNodeWidget* NewNode = AddNode(false, CharactersList[x]);
			if(NewNode)
			{
				NewRound.Add(NewNode);
			}
		}
	}
	UTurnInfoNodeWidget* NewNode = AddNode(true);
	if(NewNode)
	{
		RoundCount++;
		NewRound.Add(NewNode);
		if(NewNode->Text)
		{
			NewNode->Text->SetText(FText::AsNumber(RoundCount));
		}
	}
	TurnNodes.Add(NewRound);
}

void UTurnInfoWidget::RefreshIndices()
{
	if(TurnGridPanel)
	{
		int Index = 0;

		for (int i = TurnGridPanel->GetChildrenCount() - 1; i >= 0; --i)
		{
			UTurnInfoNodeWidget* Node = Cast<UTurnInfoNodeWidget>(TurnGridPanel->GetChildAt(i));
			if(Node && Node->IsRemoved == false)
			{
				if(ATimedEventManager::FindWidgetTransformData(Node))
				{
					return;
				}

				TArray<FWidgetTransformData*> Widgets;
				ATimedEventManager::FindWidgetTransformAsyncData(Node, Widgets);
				
				if(Widgets.Num() > 0)
				{
					return;
				}
			}
		}

		for (int i = TurnGridPanel->GetChildrenCount() - 1; i >= 0; --i)
		{
			UTurnInfoNodeWidget* Node = Cast<UTurnInfoNodeWidget>(TurnGridPanel->GetChildAt(i));
			if(Node && Node->IsRemoved)
			{
				TurnGridPanel->RemoveChildAt(i);
				Node->EmptyData();
				if(Pool.IsInitialized())
				{
					Pool.Release(Node);
				}
			}
		}
		
		// for (int i = 0; i < TurnGridPanel->GetChildrenCount(); ++i)
		// {
		// 	UTurnInfoNodeWidget* Node = Cast<UTurnInfoNodeWidget>(TurnGridPanel->GetChildAt(i));
		// 	if(Node)
		// 	{
				// ATimedEventManager::RemoveWidgetTransformTimer(Node);
				//
				// if(Node->NodePanel)
				// {
				// 	UCanvasPanelSlot* SlotData = Cast<UCanvasPanelSlot>(Node->NodePanel->Slot);
				// 	if(SlotData)
				// 	{
				// 		ATimedEventManager::RemoveCanvasPanelSlotSizeTimer(SlotData);
				// 	}
				// }
				// UGridSlot* GridSlot = Cast<UGridSlot>(Node->Slot);
				// if(GridSlot)
				// {
				// 	GridSlot->SetColumn(Index);
				// 	Index++;
				// 	Node->SetRenderTranslation(FVector2D::ZeroVector);
				// 	if(i - 1 >= 0)
				// 	{
				// 		UTurnInfoNodeWidget* LeftNode = Cast<UTurnInfoNodeWidget>(TurnGridPanel->GetChildAt(i - 1));
				// 		if(LeftNode)
				// 		{
				// 			Node->LeftNode = LeftNode;
				// 			LeftNode->RightNode = Node;
				// 		}
				// 	}
				// }

				for (int i = 0; i < TurnNodes.Num(); ++i)
				{
					for (int j = 0; j < TurnNodes[i].Num(); ++j)
					{
						if(TurnNodes[i][j])
						{
							UGridSlot* GridSlot = Cast<UGridSlot>(TurnNodes[i][j]->Slot);
							if(GridSlot)
							{
								GridSlot->SetColumn(Index);
								TurnNodes[i][j]->GridIndex = Index;
								TurnNodes[i][j]->Index = Index;
								Index++;
								TurnNodes[i][j]->SetRenderTranslation(FVector2D(0, TurnNodes[i][j]->RenderTransform.Translation.Y));
							}
						}
					}
				}
			// }
		//}
		GridIndexHead = Index;
		ActiveNodeIndex = Index;
	}
}

void UTurnInfoWidget::RecursiveSlideAll(UTurnInfoNodeWidget* Node, float PreviousWidth, float OffsetWidth, bool IsFirst, int ExpandingNodeGridIndex, float FirstNodeExpandSize)
{
	//UE_LOG(LogTemp, Warning, TEXT("Size %s"), *Node->GetDesiredSize().ToString());
	if(Node)
	{
		float ExpandTime = 0.2f;
		bool SkipLerp = Node->IsSeperator && IsFirst ? true : false;
		if(SkipLerp == false)
		{
			float SmallerThanFirstOffset = 0;
			// UE_LOG(LogTemp, Warning, TEXT("FirstGridIndex %d"), ExpandingNodeGridIndex);
			// UE_LOG(LogTemp, Warning, TEXT("Node->GridIndex %d"), Node->GridIndex);
			if(Node->GridIndex <= ExpandingNodeGridIndex)
			{
				// UE_LOG(LogTemp, Warning, TEXT("RecursiveSlideAll %f"), FirstNodeExpandSize);
				SmallerThanFirstOffset = FirstNodeExpandSize;
			}
			if(FMath::Abs(SmallerThanFirstOffset) > 0.0f)
			{
				ATimedEventManager::MoveWidgetAsync(Node, ExpandTime,FVector2D((SmallerThanFirstOffset), 0), GetWorld());
			}

			FString Key = "SlideLeft";
			ATimedEventManager::MoveWidgetAsync(Node, 0.5f,FVector2D(-(PreviousWidth + OffsetWidth), 0), GetWorld(), nullptr, &Key);
		}
		if(IsFirst)
		{
			if(Node->IsSeperator)
			{
				RemoveNodeWidget(Node, false);
				OffsetWidth += Node->GetDesiredSize().X;
				IsFirst = true;
			}
			else
			{
				if(Node->NodePanel)
				{
					UCanvasPanelSlot* SlotData = Cast<UCanvasPanelSlot>(Node->NodePanel->Slot);
					if(SlotData)
					{
						float Xratio = 1;
						FVector2D SlotSize = SlotData->GetSize();
						if(SlotSize.Y > 0)
						{
							Xratio = (SlotSize.X/SlotSize.Y);
						}
						//OffsetWidth += BigYSize * Xratio - SlotData->GetSize().X;
						ExpandingNodeGridIndex = Node->GridIndex;
						FirstNodeExpandSize = BigYSize * Xratio - SlotData->GetSize().X;
						ATimedEventManager::LerpCanvasPanelSlotSize(SlotData, ExpandTime, SlotData->GetSize(), FVector2D(BigYSize * Xratio, BigYSize),GetWorld());
						 // SlotData->SetSize(FVector2D(BigYSize * Xratio, BigYSize));
					}
				}
				IsFirst = false;
			}
		}

		if(Node && Node->RightNode)
		{
			RecursiveSlideAll(Node->RightNode, PreviousWidth, OffsetWidth, IsFirst, ExpandingNodeGridIndex, FirstNodeExpandSize);
		}
	}
}

void UTurnInfoWidget::SlideScrollbar()
{
	//TurnScrollBox->SetScrollOffset(TurnScrollBox->GetScrollOffset() + PreviousWidth + OffsetWidth);
}

void UTurnInfoWidget::Start(TArray<class AGGCharacter*> NewCharactersList)
{
	SetCharactersList(NewCharactersList);
	// for (int j = 0; j < 6; ++j)
	// {
	// 	AddNewRound();
	// }
	FillGapWithRounds();
}

void UTurnInfoWidget::UpdateOrder()
{
	if(TurnGridPanel == nullptr) return;

	for (int i = 0; i < TurnNodes.Num(); ++i)
	{
		for (int j = 0; j < TurnNodes[i].Num(); ++j)
		{
			if(TurnNodes[i][j])
			{
				TurnNodes[i][j]->IndexFirst = i;
				TurnNodes[i][j]->IndexSecond = j;
				TurnNodes[i][j]->SwapOffSet = 0;
			}
		}
	}

	UTurnInfoNodeWidget* FirstWidget = nullptr;

	if(TurnNodes.Num() > 0 && TurnNodes[0].Num() > 0 )
	{
		FirstWidget = TurnNodes[0][0];
		TurnNodes[0].RemoveAt(0);
	}

	TArray<UTurnInfoNodeWidget*> Seperators;
	for (int i = TurnNodes.Num() - 1; i >= 0 ; --i)
	{
		for (int j = TurnNodes[i].Num() - 1; j >= 0; --j)
		{
			if(TurnNodes[i][j] && TurnNodes[i][j]->Character == nullptr/* || i == 0 && j == 0*/)
			{
				Seperators.Add(TurnNodes[i][j]);
				TurnNodes[i].RemoveAt(j);
			}
		}
	}
	
	for (int i = 0; i < TurnNodes.Num(); ++i)
	{
		Algo::StableSort(TurnNodes[i], [](UTurnInfoNodeWidget* LHS, UTurnInfoNodeWidget* RHS)
		{
			if(LHS && RHS )
			{
				return LHS->Character->GetSpeed() > RHS->Character-> GetSpeed();
			}
			return false;
		});
	}

	if(TurnNodes.Num() > 0)
	{
		TurnNodes[0].Insert(FirstWidget,0);
	
		for (int i = 0; i < Seperators.Num(); ++i)
		{
			if(Seperators[i])
			{
				if(Seperators[i]->IndexFirst < TurnNodes.Num())
				{
					TurnNodes[Seperators[i]->IndexFirst].Add((Seperators[i]));
				}
			}
		}
	}

	UTurnInfoNodeWidget* LeftNode = nullptr;
	int Index = 0;
	for (int i = 0; i < TurnNodes.Num(); ++i)
	{
		for (int j = 0; j < TurnNodes[i].Num(); ++j)
		{
			if(TurnNodes[i][j])
			{
				if(LeftNode)
				{
					TurnNodes[i][j]->LeftNode = LeftNode;
					LeftNode->RightNode = TurnNodes[i][j];
				}
				LeftNode = TurnNodes[i][j];
				LeftNode->Index = Index;
				Index++;

				if(i == 0 && j == 0)
					continue;
				
				int IndexFirst = TurnNodes[i][j]->IndexFirst;
				int IndexSecond = TurnNodes[i][j]->IndexSecond;
				if((/*i != IndexFirst || */j != IndexSecond))
				{
					if(IndexFirst < TurnNodes.Num() && IndexSecond < TurnNodes[i].Num())
					{
						float Distance = (TurnNodes[i][j]->GetDesiredSize().X) * (j - IndexSecond);

						// UE_LOG(LogTemp, Warning, TEXT("Distance[%d][%d] %f"), i, j, Distance);
						TArray<FWidgetTransformData> Queue;
	
						float AnimationYValue = FMath::RandRange(50, 150);
						//AnimationYValue *= (1 - 2 *  FMath::RandRange(0, 1));
						FWidgetTransformData NewData;
						NewData.Duration = 0.5f;
						NewData.OffsetValue = FVector2D(Distance, 0);
						Queue.Add(NewData);
	
						NewData.Duration = 0.5f;
						NewData.OffsetValue = FVector2D(0, -AnimationYValue);
						Queue.Add(NewData);
						
						ATimedEventManager::MoveWidgetAsync((TurnNodes[i])[j], 0.5f, FVector2D(0, AnimationYValue),  GetWorld(), &Queue);
					}
				}
			}
		}
	}
}

void UTurnInfoWidget::RemoveDeadCharacter(AGGCharacter* CharToRemove)
{
	if(CharToRemove == nullptr) return;

	for (int i = 0; i < TurnNodes.Num(); ++i)
	{
		for (int j = TurnNodes[i].Num() - 1; j >= 0; --j)
		{
			if((TurnNodes[i])[j]->IsSeperator == false && (TurnNodes[i])[j]->Character)
			{
				if((TurnNodes[i])[j]->Character == CharToRemove)
				{
					RemoveNodeWidget((TurnNodes[i])[j], true);
				}
			}
		}
	}
}

void UTurnInfoWidget::SetCharactersList(TArray<AGGCharacter*>& NewList)
{
	for (int j = NewList.Num() - 1; j >= 0; --j)
	{
		if(NewList[j])
		{
			if(NewList[j]->GetStatus() == ECharacterStatus::Dead)
			{
				NewList.RemoveAt(j);
			}
		}
	}
	CharactersList = NewList;
}
