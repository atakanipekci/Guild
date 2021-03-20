// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggedCharacterWidget.h"



#include "CharacterDetailWidget.h"
#include "CharacterStats.h"
#include "DroppableAreaWidget.h"
#include "TownGameModeBase.h"
#include "WidgetManager.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UDraggedCharacterWidget::NativeConstruct()
{
    ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));

   // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CONSTRUCTOR Name %s"), *CharacterNameText->Text.ToString()));


    // if(GameMode->CharactersTable && Stat == nullptr)
    // {
    //     const FString Context(TEXT("CONTEXT DATATABLE TEXT"));
    //     const FName Row = *(FString(TEXT("Knight")));
    //
    //     FCharacterStats* CharacterData = GameMode->CharactersTable->FindRow<FCharacterStats>(Row, Context, true);
    //
    //     if(CharacterData)
    //     {
    //         CharacterNameText->SetText(FText::FromString(FString::FromInt(WidgetManager::GetAndSetDraggableSpawnCount())));
    //         FCharacterStats* CopyStruct = new FCharacterStats(*CharacterData);
    //         CopyStruct->Price = 50;
    //         Stat = CopyStruct;
    //
    //         //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DRAGGABLE NAME %s"), *this->GetName()));
    //     }
    // }
}

void UDraggedCharacterWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    if(OwnerDroppableArea)
    {
        if(OwnerDroppableArea->ContentPanel)
        {
            this->SetVisibility(ESlateVisibility::Visible);

            if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::OwnedCharacters)
            {
                OwnerDroppableArea->ContentPanel->InsertChildAt(DraggedIndex, this);
                LatestChildIndex = DraggedIndex;

                
                TArray<UWidget*> Childs;
                UPanelWidget* Parent = this->GetParent();

                if(Parent)
                {
                    for (int i = 0; i < Parent->GetChildrenCount(); ++i)
                    {
                        UWidget* Child = Parent->GetChildAt(i);
                        Childs.Add(Child);
                    }
                    Parent->ClearChildren();
                    for (int i = 0; i < Childs.Num(); ++i)
                    {
                        Parent->AddChild(Childs[i]);
                    }
                }
            }
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DRAG CANCEL INDEX %d"), DraggedIndex ));
        }
    }
}

void UDraggedCharacterWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
     if(InOperation->Payload && this->OwnerDroppableArea != nullptr)
    {
         if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::OwnedCharacters)
         {
            UDraggedCharacterWidget* DraggedWidget = Cast<UDraggedCharacterWidget>(InOperation->Payload);
            if(DraggedWidget)
            {
               if(this->GetParent() && DraggedWidget != this)
               {
			       bool bShouldResetList = true;
                  // if(DraggedWidget->GetParent() != nullptr)
                  // {
                      if(DraggedWidget->GetParent() == this->GetParent())
                      {
                          //bShouldResetList = false;
                           this->GetParent()->ShiftChild(this->GetParent()->GetChildIndex(this), DraggedWidget);
                      }
                      else
                      {
                           this->GetParent()->InsertChildAt(this->GetParent()->GetChildIndex(this), DraggedWidget);
                      }
                  // }
                  if(bShouldResetList == true)
                  {
                        DraggedWidget->SetVisibility(ESlateVisibility::Hidden);
                       TArray<UWidget*> Childs;
                       UPanelWidget* Parent = this->GetParent();
                      if(Parent)
                      {
                          for (int i = 0; i < Parent->GetChildrenCount(); ++i)
                          {
                              UWidget* Child = Parent->GetChildAt(i);
                              Childs.Add(Child);
                          }
                          Parent->ClearChildren();
                          for (int i = 0; i < Childs.Num(); ++i)
                          {
                              Parent->AddChild(Childs[i]);
                          }
                      }
                  }

                  DraggedWidget->DraggedIndex = DraggedWidget->GetParent()->GetChildIndex(DraggedWidget);
                  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ONENTER DRAGINDEX %d"), DraggedIndex));
                 // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Child Index %d"), GetParent()->GetChildIndex(DraggedWidget)));
                  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Child Count %d"), GetParent()->GetChildrenCount()));
               }
            }
            else
            {
               
            }
         }
    }
}

void UDraggedCharacterWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
    UDragDropOperation*& OutOperation)
{
    SetVisibility(ESlateVisibility::Hidden);
    if(DraggedVisualInstance == nullptr)
    {
        DraggedVisualInstance = CreateWidget<UUserWidget>(GetWorld(), DraggedVisualBp);
    }

    OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperation);
    OutOperation->Payload = this;
    OutOperation->DefaultDragVisual = DraggedVisualInstance;
}

FReply UDraggedCharacterWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		 GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Right Mouse Button Down"));

        UUserWidget* NewWidget = WidgetManager::GetWidgetInstance(EWidgetKeys::CharacterDetail);
        
        if(NewWidget == nullptr)
        {
            NewWidget = CreateWidget<UUserWidget>(this->GetWorld(), WidgetManager::GetWidget(EWidgetKeys::CharacterDetail));
            WidgetManager::SetWidgetInstance(EWidgetKeys::CharacterDetail, NewWidget);
        }
        else
        {
            NewWidget->RemoveFromViewport();
        }

        NewWidget->SetVisibility(ESlateVisibility::Visible);
        NewWidget->AddToViewport();
	}

    const FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    return Reply.NativeReply;
}

void UDraggedCharacterWidget::OnDragLeaveFromArea()
{
     if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::OwnedCharacters)
     {
         this->RemoveFromParent();
         DraggedIndex = LatestChildIndex;
     }
}


void UDraggedCharacterWidget::SetOwnerAreaWidget(UDroppableAreaWidget* Owner)
{
    OwnerDroppableArea = Owner;
}

void UDraggedCharacterWidget::SetCharacterNameText(FText Text)
{
    if(CharacterNameText)
    {
        CharacterNameText->SetText(Text);
    }
}

void UDraggedCharacterWidget::SetStat(FCharacterStats* NewStat)
{
    Stat = NewStat;
    if(Stat)
    {
        SetCharacterNameText(FText::FromString(FString::FromInt(Stat->UniqueID)));
    }
}
