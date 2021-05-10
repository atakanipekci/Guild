// Fill out your copyright notice in the Description page of Project Settings.


#include "DayTaskManager.h"


#include "GuildGameInstance.h"
#include "Kismet/GameplayStatics.h"

TMap<FString, FDayTask* > DayTaskManager::Tasks;
TMap<FString, ITaskCaller* > DayTaskManager::Buildings;


DayTaskManager::DayTaskManager()
{
    Tasks.Empty();
}

DayTaskManager::~DayTaskManager()
{
}

void DayTaskManager::AddTask(FString Key, int StartDay, int DaysToPass, FTaskStart& OnStart, FTaskFinish& OnFinish,
    FTaskUpdate& OnUpdate)
{
   /* UE_LOG(LogTemp, Warning, TEXT("ADDDD"));

    if(Tasks.Contains(Key))
    {
        UE_LOG(LogTemp, Error, TEXT("There is Already a Task with a This Key : %s"), *Key);
    }
    else
    {
        FDayTask* NewTask = new FDayTask();
        NewTask->OnFinish = &OnFinish;
        NewTask->OnStart = &OnStart;
        NewTask->OnUpdate = &OnUpdate;
        NewTask->StartDay = StartDay;
        NewTask->DaysToPass = DaysToPass;

    
        if(OnStart.IsBound())
           OnStart.Execute();

        Tasks.Add(Key, NewTask);
    }*/
}

/*void DayTaskManager::UpdateTasks(UObject* UObj)
{

  TArray<FString> FinishedTasks;
    UE_LOG(LogTemp, Warning, TEXT("Update"));
  for (const TPair<FString, FDayTask*>& Pair : Tasks)
  {
      FDayTask* Item = Pair.Value;
      if(Item)
      {
          UE_LOG(LogTemp, Warning, TEXT("KEY , %s"),*(Pair.Key));
          if(Item->OnUpdate)
          {
              if(Item->OnUpdate->IsBound())
                  Item->OnUpdate->Execute();
          }

          if(UObj)
          {
               UTownGameInstance* GameInstance = Cast<UTownGameInstance>(UGameplayStatics::GetGameInstance(UObj->GetWorld()));

              if(GameInstance)
              {
                  UE_LOG(LogTemp, Warning, TEXT("IS INSIDE IF"));

                 if(Item->StartDay + Item->DaysToPass <= GameInstance->Day)
                 {
                     UE_LOG(LogTemp, Warning, TEXT("IS BOUND"));

                     if(Item->OnFinish)
                     {
                         if(Item->OnFinish->IsBound())
                         {
                             UE_LOG(LogTemp, Warning, TEXT("YES ITS BOUND"));
                             Item->OnFinish->Execute();
                             FinishedTasks.Add(Pair.Key);
                         }
                     }
                 }
              }
          }
      }
  }

  for (int i = 0; i < FinishedTasks.Num(); ++i)
  {
        Tasks.Remove(FinishedTasks[i]);
       UE_LOG(LogTemp, Warning, TEXT("REMOVED"));
  }
}*/

void DayTaskManager::UpdateTasks(UObject* UObj)
{

  TArray<FString> FinishedTasks;
    UE_LOG(LogTemp, Warning, TEXT("Update"));
  for (const TPair<FString, FDayTask*>& Pair : Tasks)
  {
      FDayTask* Item = Pair.Value;
      if(Item)
      {
          ITaskCaller* TaskCaller = GetTaskCaller(Pair.Key, Item->TaskType);
            UE_LOG(LogTemp, Warning, TEXT("ITEM NOT NULL"));
          if(TaskCaller)
          {
              UE_LOG(LogTemp, Warning, TEXT("TASK NOT NULL"));

              TaskCaller->OnTaskUpdate();

              if(UObj)
              {
                   UGuildGameInstance* GameInstance = Cast<UGuildGameInstance>(UGameplayStatics::GetGameInstance(UObj->GetWorld()));

                  if(GameInstance)
                  {
                      UE_LOG(LogTemp, Warning, TEXT("IS INSIDE IF"));

                     if(Item->StartDay + Item->DaysToPass <= GameInstance->Day)
                     {
                         UE_LOG(LogTemp, Warning, TEXT("TASKFINISHED"));
                         TaskCaller->OnTaskFinish();
                         FinishedTasks.Add(Pair.Key);
                     }
                  }
              }
          }
      }
  }

  for (int i = 0; i < FinishedTasks.Num(); ++i)
  {
        Tasks.Remove(FinishedTasks[i]);
       UE_LOG(LogTemp, Warning, TEXT("REMOVED"));
  }
}
void DayTaskManager::AddTask(FString Key, int StartDay, int DaysToPass, ETaskType TaskType)
{
     UE_LOG(LogTemp, Warning, TEXT("ADDDD"));

    if(Tasks.Contains(Key))
    {
        UE_LOG(LogTemp, Error, TEXT("There is Already a Task with a This Key : %s"), *Key);
    }
    else
    {
        FDayTask* NewTask = new FDayTask();
        NewTask->StartDay = StartDay;
        NewTask->DaysToPass = DaysToPass;
        NewTask->TaskType = TaskType;

        Tasks.Add(Key, NewTask);
    }

}

void DayTaskManager::AddBuildingCaller(FString Key, ITaskCaller* Caller)
{
    Buildings.Add(Key, Caller);
}

ITaskCaller* DayTaskManager::GetTaskCaller(FString Key, ETaskType TaskType)
{
    if(TaskType == ETaskType::ConstructionBuilding || TaskType == ETaskType::UpgradeBuilding)
    {
        if(Buildings.Contains(Key))
        {
            ITaskCaller** TaskCaller = Buildings.Find(Key);
            if(TaskCaller !=nullptr)
            {
                if(*TaskCaller != nullptr)
                {
                    return *TaskCaller;
                }
            }
            
        }
    }

    return nullptr;
}
