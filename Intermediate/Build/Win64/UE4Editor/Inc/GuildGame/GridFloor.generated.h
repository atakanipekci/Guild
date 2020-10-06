// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GUILDGAME_GridFloor_generated_h
#error "GridFloor.generated.h already included, missing '#pragma once' in GridFloor.h"
#endif
#define GUILDGAME_GridFloor_generated_h

#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_SPARSE_DATA
#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_RPC_WRAPPERS
#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_RPC_WRAPPERS_NO_PURE_DECLS
#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAGridFloor(); \
	friend struct Z_Construct_UClass_AGridFloor_Statics; \
public: \
	DECLARE_CLASS(AGridFloor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/GuildGame"), NO_API) \
	DECLARE_SERIALIZER(AGridFloor)


#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_INCLASS \
private: \
	static void StaticRegisterNativesAGridFloor(); \
	friend struct Z_Construct_UClass_AGridFloor_Statics; \
public: \
	DECLARE_CLASS(AGridFloor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/GuildGame"), NO_API) \
	DECLARE_SERIALIZER(AGridFloor)


#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AGridFloor(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AGridFloor) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AGridFloor); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AGridFloor); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AGridFloor(AGridFloor&&); \
	NO_API AGridFloor(const AGridFloor&); \
public:


#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AGridFloor(AGridFloor&&); \
	NO_API AGridFloor(const AGridFloor&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AGridFloor); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AGridFloor); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AGridFloor)


#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__GridFloorTypeCount() { return STRUCT_OFFSET(AGridFloor, GridFloorTypeCount); }


#define GuildGame_Source_GuildGame_Public_GridFloor_h_13_PROLOG
#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_PRIVATE_PROPERTY_OFFSET \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_SPARSE_DATA \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_RPC_WRAPPERS \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_INCLASS \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define GuildGame_Source_GuildGame_Public_GridFloor_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_PRIVATE_PROPERTY_OFFSET \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_SPARSE_DATA \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_INCLASS_NO_PURE_DECLS \
	GuildGame_Source_GuildGame_Public_GridFloor_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GUILDGAME_API UClass* StaticClass<class AGridFloor>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID GuildGame_Source_GuildGame_Public_GridFloor_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
