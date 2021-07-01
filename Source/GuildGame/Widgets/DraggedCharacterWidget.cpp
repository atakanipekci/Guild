// Fill out your copyright notice in the Description page of Project Settings.


#include "DraggedCharacterWidget.h"



#include "CharacterDetailWidget.h"
#include "DraggedVisualWidget.h"
#include "GuildGame/Characters/CharacterStats.h"
#include "DroppableAreaWidget.h"
#include "OwnedCharactersDroppableWidget.h"
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
        OwnerDroppableArea->OnChildDraggableDragCancelled(this, PreviousChildIndex);
    }
}

void UDraggedCharacterWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
    UDragDropOperation* InOperation)
{
    Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

     if(InOperation->Payload && this->OwnerDroppableArea && this->OwnerDroppableArea->ContentPanel)
    {
        UDraggedCharacterWidget* DraggedWidget = Cast<UDraggedCharacterWidget>(InOperation->Payload);
        if(DraggedWidget)
        {
            OwnerDroppableArea->OnDragEnterIntoAnotherDraggable(DraggedWidget, this);
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
        UUserWidget* NewWidget = AWidgetManager::GetOrCreateWidgetInstance(EWidgetKeys::CharacterDetail, GetWorld());
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
    if(this->OwnerDroppableArea == nullptr || Area == nullptr)
        return;

   
     if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::OwnedCharacters && Area->AreaType == EDroppableAreaType::OwnedCharacters)
     {
         // this->RemoveFromParent();
        if(this->GetParent())
        {
             SetPreviousChildIndex(this->GetParent()->GetChildIndex(this));
        }
        //this->OwnerDroppableArea->RemoveFromDroppableArea(this);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REMOVED FROM Owned")));

     }
    else if(this->OwnerDroppableArea->AreaType == EDroppableAreaType::RecruitableCharacters && Area->AreaType == EDroppableAreaType::RecruitableCharacters)
    {
        if(this->GetParent())
        {
             SetPreviousChildIndex(this->GetParent()->GetChildIndex(this));
        }
        this->OwnerDroppableArea->RemoveFromDroppableArea(this);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REMOVED FROM RECR %d"), PreviousChildIndex));
    }
    else if(Area->AreaType == EDroppableAreaType::OwnedCharacters && this->OwnerDroppableArea->AreaType != EDroppableAreaType::OwnedCharacters)
    {
        Area->RemoveFromDroppableArea(this);//Removes The DraggedCopy
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("REMOVED FROM Owned but parent not Owned  %d"), PreviousChildIndex ));
    }

     GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OnDragLeaveFromArea")));
    // Area->UpdateChildIndices();
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

int UDraggedCharacterWidget::GetPreviousChildIndex()
{
    return PreviousChildIndex;
}
