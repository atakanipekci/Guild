#pragma once


#include "CharacterStats.h"
#include "CoreMinimal.h"

GUILDGAME_API class ImageManager
{
public:
	static UTexture* GetPortraitTextureByClass(ECharacterClassType ClassType);
	static void SetPortraitTextureByClass(ECharacterClassType ClassType, class UImage* Image);


	
	
	
};
