#include "ImageManager.h"

#include "Components/Image.h"

UTexture* ImageManager::GetPortraitTextureByClass(ECharacterClassType ClassType)
{
	if(ClassType == ECharacterClassType::Knight)
	{
		FString Path = FString("/Game/Sprites/UI/Icons/skill_icon_04");
		UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));

		return Texture;
	}
	else 
	{
		FString Path = FString("/Game/Sprites/UI/Icons/skill_icon_03");
		UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));

		return Texture;
	}
	
}

void ImageManager::SetPortraitTextureByClass(ECharacterClassType ClassType, UImage* Image)
{
	UTexture* Texture = GetPortraitTextureByClass(ClassType);
	Image->SetBrushResourceObject(Texture);

}
