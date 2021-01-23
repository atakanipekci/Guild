// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DayTaskManager.generated.h"


DECLARE_DELEGATE(FTaskStart);
DECLARE_DELEGATE(FTaskFinish);
DECLARE_DELEGATE(FTaskUpdate);

/**
 * 
 */

UENUM()
enum class ETaskType:uint8
{
	ConstructionBuilding,
	UpgradeBuilding
};

USTRUCT()
struct FDayTask
{
    GENERATED_BODY()
	
	int StartDay;
	int DaysToPass;

	FTaskStart* OnStart;
	FTaskFinish* OnFinish;
	FTaskUpdate* OnUpdate;

	ETaskType TaskType;
};

class GUILDGAME_API ITaskCaller
{
    public:
	virtual void OnTaskStart() = 0;
	virtual void OnTaskFinish() = 0;
	virtual void OnTaskUpdate() = 0;
	virtual ~ITaskCaller() = default;
};

class GUILDGAME_API DayTaskManager
{
public:
	DayTaskManager();
	~DayTaskManager();

	static void AddTask(FString Key,int StartDay, int DaysToPass, FTaskStart& OnStart, FTaskFinish& OnFinish, FTaskUpdate& OnUpdate);

	static void UpdateTasks(UObject* UObj);

	static void AddTask(FString Key,int StartDay, int DaysToPass, ETaskType TaskType);

	static void AddBuildingCaller(FString Key, ITaskCaller* Caller);
	static ITaskCaller* GetTaskCaller(FString Key, ETaskType TaskType);

	static TMap<FString, FDayTask*> Tasks;
	static TMap<FString, ITaskCaller*> Buildings;

};
