// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GUILDGAME_GGPawn_generated_h
#error "GGPawn.generated.h already included, missing '#pragma once' in GGPawn.h"
#endif
#define GUILDGAME_GGPawn_generated_h

#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_SPARSE_DATA
#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_RPC_WRAPPERS
#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAGGPawn(); \
	friend struct Z_Construct_UClass_AGGPawn_Statics; \
public: \
	DECLARE_CLASS(AGGPawn, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/GuildGame"), NO_API) \
	DECLARE_SERIALIZER(AGGPawn)


#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAGGPawn(); \
	friend struct Z_Construct_UClass_AGGPawn_Statics; \
public: \
	DECLARE_CLASS(AGGPawn, APawn, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/GuildGame"), NO_API) \
	DECLARE_SERIALIZER(AGGPawn)


#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AGGPawn(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AGGPawn) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AGGPawn); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AGGPawn); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AGGPawn(AGGPawn&&); \
	NO_API AGGPawn(const AGGPawn&); \
public:


#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AGGPawn(AGGPawn&&); \
	NO_API AGGPawn(const AGGPawn&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AGGPawn); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AGGPawn); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AGGPawn)


#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_PRIVATE_PROPERTY_OFFSET
#define GuildGame_Source_GuildGame_Public_GGPawn_h_9_PROLOG
#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_PRIVATE_PROPERTY_OFFSET \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_SPARSE_DATA \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_RPC_WRAPPERS \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_INCLASS \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define GuildGame_Source_GuildGame_Public_GGPawn_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_PRIVATE_PROPERTY_OFFSET \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_SPARSE_DATA \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_INCLASS_NO_PURE_DECLS \
	GuildGame_Source_GuildGame_Public_GGPawn_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GUILDGAME_API UClass* StaticClass<class AGGPawn>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID GuildGame_Source_GuildGame_Public_GGPawn_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
