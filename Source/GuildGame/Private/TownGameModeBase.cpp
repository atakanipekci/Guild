// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameModeBase.h"

#include "TownPlayerController.h"

ATownGameModeBase::ATownGameModeBase()
{
    PlayerControllerClass = ATownPlayerController::StaticClass();
}
