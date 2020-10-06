#pragma once

#include "CoreMinimal.h"

#define LOG(txt, ...) UE_LOG(LogTemp, Log, TEXT(txt), __VA_ARGS__)
#define LOG_W(txt, ...) UE_LOG(LogTemp, Warning, TEXT(txt), __VA_ARGS__)
#define LOG_ERR(txt, ...) UE_LOG(LogTemp, Error, TEXT(txt), __VA_ARGS__)