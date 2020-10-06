// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GuildGame/Public/GridNavigationData.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGridNavigationData() {}
// Cross Module References
	GUILDGAME_API UClass* Z_Construct_UClass_AGridNavigationData_NoRegister();
	GUILDGAME_API UClass* Z_Construct_UClass_AGridNavigationData();
	NAVIGATIONSYSTEM_API UClass* Z_Construct_UClass_ANavigationData();
	UPackage* Z_Construct_UPackage__Script_GuildGame();
// End Cross Module References
	void AGridNavigationData::StaticRegisterNativesAGridNavigationData()
	{
	}
	UClass* Z_Construct_UClass_AGridNavigationData_NoRegister()
	{
		return AGridNavigationData::StaticClass();
	}
	struct Z_Construct_UClass_AGridNavigationData_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AGridNavigationData_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_ANavigationData,
		(UObject* (*)())Z_Construct_UPackage__Script_GuildGame,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AGridNavigationData_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Input Rendering Tags Utilities|Transformation Actor Layers Replication" },
		{ "IncludePath", "GridNavigationData.h" },
		{ "ModuleRelativePath", "Public/GridNavigationData.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AGridNavigationData_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AGridNavigationData>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AGridNavigationData_Statics::ClassParams = {
		&AGridNavigationData::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002A6u,
		METADATA_PARAMS(Z_Construct_UClass_AGridNavigationData_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AGridNavigationData_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AGridNavigationData()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AGridNavigationData_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AGridNavigationData, 997476265);
	template<> GUILDGAME_API UClass* StaticClass<AGridNavigationData>()
	{
		return AGridNavigationData::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AGridNavigationData(Z_Construct_UClass_AGridNavigationData, &AGridNavigationData::StaticClass, TEXT("/Script/GuildGame"), TEXT("AGridNavigationData"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AGridNavigationData);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
