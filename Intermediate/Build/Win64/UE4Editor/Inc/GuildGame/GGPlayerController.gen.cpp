// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GuildGame/Public/GGPlayerController.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGGPlayerController() {}
// Cross Module References
	GUILDGAME_API UClass* Z_Construct_UClass_AGGPlayerController_NoRegister();
	GUILDGAME_API UClass* Z_Construct_UClass_AGGPlayerController();
	ENGINE_API UClass* Z_Construct_UClass_APlayerController();
	UPackage* Z_Construct_UPackage__Script_GuildGame();
// End Cross Module References
	void AGGPlayerController::StaticRegisterNativesAGGPlayerController()
	{
	}
	UClass* Z_Construct_UClass_AGGPlayerController_NoRegister()
	{
		return AGGPlayerController::StaticClass();
	}
	struct Z_Construct_UClass_AGGPlayerController_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGGPlayerController_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APlayerController,
		(UObject* (*)())Z_Construct_UPackage__Script_GuildGame,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGGPlayerController_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "GGPlayerController.h" },
		{ "ModuleRelativePath", "Public/GGPlayerController.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGGPlayerController_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGGPlayerController>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AGGPlayerController_Statics::ClassParams = {
		&AGGPlayerController::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002A4u,
		METADATA_PARAMS(Z_Construct_UClass_AGGPlayerController_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AGGPlayerController_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AGGPlayerController()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AGGPlayerController_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AGGPlayerController, 1199178789);
	template<> GUILDGAME_API UClass* StaticClass<AGGPlayerController>()
	{
		return AGGPlayerController::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AGGPlayerController(Z_Construct_UClass_AGGPlayerController, &AGGPlayerController::StaticClass, TEXT("/Script/GuildGame"), TEXT("AGGPlayerController"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGGPlayerController);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
