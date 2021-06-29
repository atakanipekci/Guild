// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggedCharacterWidget.h"



#include "CharacterDetailWidget.h"
#include "DraggedVisualWidget.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "DroppableAreaWidget.h"
#include "GuildGame/Town/TownGameModeBase.h"
#include "GuildGame/Managers/WidgetManager.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "GuildGame/Managers/ImageManager.h"
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
    Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

    if(OwnerDroppableArea)
    {
        if(OwnerDroppableArea->ContentPanel)
        {
            this->SetVisibility(ESlateVisibility::Visible);

            if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::OwnedCharacters || this->OwnerDroppableArea->AreaType == EDroppableAreaType::RecruitableCharacters)
            {
                if(OwnerDroppableArea->ContentPanel->HasChild(this) == false)
                {
                    OwnerDroppableArea->ContentPanel->InsertChildAt(PreviousChildIndex, this);
                    OwnerDroppableArea->UpdateChildIndices();

                    
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
               
            }
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DRAG CANCEL INDEX %d"), PreviousChildIndex ));
        }
    }
}

void UDraggedCharacterWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

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
                  int StartIndex = 0;
                  if(DraggedWidget->GetParent() == this->GetParent())
                  {
                      int MyIndex = this->GetParent()->GetChildIndex(this);
                      int DraggedWidgetIndex = DraggedWidget->GetParent()->GetChildIndex(DraggedWidget);
                      //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("SWAP %d"), ));
                      StartIndex = MyIndex <= DraggedWidgetIndex ? MyIndex : DraggedWidgetIndex;
                       this->GetParent()->ShiftChild(this->GetParent()->GetChildIndex(this), DraggedWidget);

                      if(DraggedWidget->OwnerDroppableArea && DraggedWidget->OwnerDroppableArea->AreaType == EDroppableAreaType::OwnedCharacters)
                      {
                          DraggedWidget->SetPreviousChildIndex(MyIndex);
                          this->SetPreviousChildIndex(DraggedWidgetIndex);
                      }
                  }
                  else
                  {
                      StartIndex = this->GetParent()->GetChildIndex(this);
                      this->GetParent()->InsertChildAt(this->GetParent()->GetChildIndex(this), DraggedWidget);
                  }
                  if(bShouldResetList == true)
                  {
                      DraggedWidget->SetVisibility(ESlateVisibility::Hidden);
                      TArray<UWidget*> Childs;
                      UPanelWidget* Parent = this->GetParent();
                      if(Parent)
                      {
                          for (int i = StartIndex; i < Parent->GetChildrenCount(); ++i)
                          {
                              UWidget* Child = Parent->GetChildAt(i);
                              Childs.Add(Child);
                          }
                          for (int i = Parent->GetChildrenCount() - 1; i >= StartIndex; --i)
                          {
                              Parent->RemoveChildAt(i);
                          }
                          for (int i = 0; i < Childs.Num(); ++i)
                          {
                              if(i != Childs.Num() - 1)
                              {
                                Parent->InsertChildAt(Parent->GetChildrenCount(), Childs[i]);
                              }
                              else
                              {
                                  Parent->AddChild(Childs[i]);
                              }
                          }
                      }
                  }

                  // DraggedWidget->SetDraggedIndex(DraggedWidget->GetParent()->GetChildIndex(DraggedWidget));
                  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ONENTER DRAGINDEX %d"), PreviousChildIndex));
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
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    SetVisibility(ESlateVisibility::Hidden);
    if(DraggedVisualInstance == nullptr)
    {
        DraggedVisualInstance = CreateWidget<UUserWidget>(GetWorld(), DraggedVisualBp);
    }

    OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperation);
    OutOperation->Payload = this;
    OutOperation->DefaultDragVisual = DraggedVisualInstance;

    if(DraggedVisualInstance)
    {
        UDraggedVisualWidget* Visual = Cast<UDraggedVisualWidget>(DraggedVisualInstance);

        if(Visual)
        {
            if(Visual->Portrait && Stat)
            {
                ImageManager::SetPortraitTextureByClass(Stat->ClassType, Visual->Portrait);
            }
        }
        
    }
    //OutOperation->event
}

FReply UDraggedCharacterWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    if(InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
        UUserWidget* NewWidget = WidgetManager::GetOrCreateWidgetInstance(EWidgetKeys::CharacterDetail, this);
        if(NewWidget)
        {
            //NewWidget->RemoveFromViewport();
            NewWidget->SetVisibility(ESlateVisibility::Visible);
            if(NewWidget->IsInViewport() == false)
            {
                NewWidget->AddToViewport();
            }
        }
        UCharacterDetailWidget* ChrDetail =  Cast<UCharacterDetailWidget>(NewWidget);
        if(ChrDetail)
        {
            ChrDetail->RefreshPage(Stat);
        }
	}

    const FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    return Reply.NativeReply;
}

void UDraggedCharacterWidget::OnDragLeaveFromArea(UDroppableAreaWidget* Area)
{
    if(this->OwnerDroppableArea == nullptr || Area == nullptr || this->GetParent() == nullptr)
        return;
    
     if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::OwnedCharacters && Area->AreaType == EDroppableAreaType::OwnedCharacters)
     {
         SetPreviousChildIndex(this->GetParent()->GetChildIndex(this));
         this->RemoveFromParent();
         Area->UpdateChildIndices();
     }
    else if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::RecruitableCharacters && Area->AreaType == EDroppableAreaType::RecruitableCharacters)
    {
        SetPreviousChildIndex(this->GetParent()->GetChildIndex(this));
        this->RemoveFromParent();
        Area->UpdateChildIndices();
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REMOVED FROM RECR %d"), PreviousChildIndex ));
    }
    else if(Area->AreaType == EDroppableAreaType::OwnedCharacters)
    {
        this->RemoveFromParent();
        Area->UpdateChildIndices();
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REMOVED FROM Owned but parent not Owned  %d"), PreviousChildIndex ));
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
        SetCharacterNameText(FText::FromString(FString::FromInt(Stat->UniqueID)+ " " + Stat->ClassName));
        // SetCharacterNameText(FText::FromString(FString::FromInt(PreviousChildIndex)+ " " + Stat->ClassName));
    }
}

void UDraggedCharacterWidget::SetPreviousChildIndex(int NewIndex)
{
    PreviousChildIndex = NewIndex;
    //SetCharacterNameText(FText::FromString(FString::FromInt(PreviousChildIndex)+ " " + Stat->ClassName));
}
