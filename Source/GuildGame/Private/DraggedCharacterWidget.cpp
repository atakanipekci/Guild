// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggedCharacterWidget.h"



#include "CharacterStats.h"
#include "DroppableAreaWidget.h"
#include "TownGameModeBase.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

void UDraggedCharacterWidget::NativeConstruct()
{
    ATownGameModeBase* GameMode = Cast<ATownGameModeBase>(UGameplayStatics::GetGameMode(this));

    if(GameMode->CharactersTable)
    {
        const FString Context(TEXT("CONTEXT DATATABLE TEXT"));
        const FName Row = *(FString(TEXT("Knight")));
        FCharacterStats* CharacterData = GameMode->CharactersTable->FindRow<FCharacterStats>(Row, Context, true);

        if(CharacterData)
        {
            FCharacterStats* CopyStruct = new FCharacterStats(*CharacterData);
            CopyStruct->Price = 50;
            Stat = CopyStruct;
        }
    }
}

void UDraggedCharacterWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    if(OwnerDroppableArea)
    {
        if(OwnerDroppableArea->ContentPanel)
        {
            OwnerDroppableArea->ContentPanel->InsertChildAt(0, this);
            this->SetVisibility(ESlateVisibility::Visible);

        }
    }
}

void UDraggedCharacterWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
     if(InOperation->Payload)
    {
        UDraggedCharacterWidget* DraggedWidget = Cast<UDraggedCharacterWidget>(InOperation->Payload);
        if(DraggedWidget)
        {
           if(this->GetParent() && DraggedWidget != this)
           {
				bool bShouldResetList = true;
               if(DraggedWidget->GetParent() != nullptr)
               {
                   if(DraggedWidget->GetParent() == this->GetParent())
                   {
                       bShouldResetList = false;
                       //this->GetParent()->ShiftChild(this->GetParent()->GetChildIndex(this), DraggedWidget);
                   }
               }
              
               // this->GetParent()->InsertChildAt(this->GetParent()->GetChildIndex(this), DraggedWidget);
               //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Added To Index %d"), this->GetParent()->GetChildIndex(DraggedWidget)));
                
                //DraggedWidget->SetVisibility(ESlateVisibility::Hidden);
               /*if(bShouldResetList == true)
               {
                TArray<UWidget*> Childs;
                UPanelWidget* Parent = this->GetParent();
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
                   
               }*/
               
               GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Child Count %d"), GetParent()->GetChildIndex(DraggedWidget)));


               
           }
        }
        else
        {
           
        }
    }
}


void UDraggedCharacterWidget::SetOwnerAreaWidget(UDroppableAreaWidget* Owner)
{
    OwnerDroppableArea = Owner;
}
