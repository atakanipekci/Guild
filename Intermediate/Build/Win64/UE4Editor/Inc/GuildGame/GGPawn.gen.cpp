// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GuildGame/Public/GGPawn.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGGPawn() {}
// Cross Module References
	GUILDGAME_API UClass* Z_Construct_UClass_AGGPawn_NoRegister();
	GUILDGAME_API UClass* Z_Construct_UClass_AGGPawn();
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	UPackage* Z_Construct_UPackage__Script_GuildGame();
// End Cross Module References
	void AGGPawn::StaticRegisterNativesAGGPawn()
	{
	}
	UClass* Z_Construct_UClass_AGGPawn_NoRegister()
	{
		return AGGPawn::StaticClass();
	}
	struct Z_Construct_UClass_AGGPawn_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGGPawn_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APawn,
		(UObject* (*)())Z_Construct_UPackage__Script_GuildGame,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGGPawn_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "GGPawn.h" },
		{ "ModuleRelativePath", "Public/GGPawn.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGGPawn_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGGPawn>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AGGPawn_Statics::ClassParams = {
		&AGGPawn::StaticClass,
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
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AGGPawn_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AGGPawn_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AGGPawn()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AGGPawn_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AGGPawn, 2434697685);
	template<> GUILDGAME_API UClass* StaticClass<AGGPawn>()
	{
		return AGGPawn::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AGGPawn(Z_Construct_UClass_AGGPawn, &AGGPawn::StaticClass, TEXT("/Script/GuildGame"), TEXT("AGGPawn"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGGPawn);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
