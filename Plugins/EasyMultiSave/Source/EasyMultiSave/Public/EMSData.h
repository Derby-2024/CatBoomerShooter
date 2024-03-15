//Easy Multi Save - Copyright (C) 2024 by Michael Hegemann.  

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "EMSData.generated.h"

#define EMS_VERSION_NUMBER 151

#define EMS_ENGINE_MIN_UE53 (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)

#define EMS_SHORT_DELAY 0.1f
#define EMS_LONG_DELAY 1.0f

namespace EMS
{
	static const FName HasLoadedTag(TEXT("EMS_HasLoaded"));
	static const FName SkipSaveTag(TEXT("EMS_SkipSave"));
	static const FName PersistentTag(TEXT("EMS_Persistent"));
	static const FName SkipTransformTag(TEXT("EMS_SkipTransform"));
	static const FName PlayerPawnAsLevelActorTag(TEXT("EMS_PawnAsLevelActor"));

	static const FString Underscore(TEXT("_"));
	static const FString Slash(TEXT("/"));
	static const FString PlayerSuffix(TEXT("Player"));
	static const FString ActorSuffix(TEXT("Level"));
	static const FString SlotSuffix(TEXT("Slot"));
	static const FString SaveGamesFolder(TEXT("SaveGames/"));
	static const FString SaveType(TEXT(".sav"));

	static const FString ImgFormatPNG(TEXT("png"));
	static const FString ImgFormatJPG(TEXT("jpg"));

	template <typename TArrayType>
	FORCEINLINE static bool ArrayEmpty(const TArrayType& InArray) { return InArray.Num() <= 0; }
}

UENUM()
enum class EUpdateActorResult: uint8
{
	RES_Success,
	RES_Skip,
	RES_ShouldSpawnNewActor,
};

UENUM()
enum class EDataLoadType : uint8
{
	DATA_Level,
	DATA_Player, 
	DATA_Object,
};

UENUM()
enum class EActorType : uint8
{
	AT_Runtime,           
	AT_Placed,           
	AT_LevelScript,       
	AT_Player,
	AT_GameObject,
	AT_Persistent,
};

UENUM()
enum class ESaveGameMode : uint8
{
	MODE_Player,
	MODE_Level,
	MODE_All,
};

UENUM()
enum class EAsyncCheckType : uint8
{
	CT_Both UMETA(DisplayName = "Both"),
	CT_Save UMETA(DisplayName = "Save Only"),
	CT_Load UMETA(DisplayName = "Load Only"),
};

UENUM(BlueprintType)
enum class ESaveErrorType : uint8
{
	ER_Player UMETA(DisplayName = "Player Actors"), 
	ER_Level  UMETA(DisplayName = "Level Actors"), 
};

UENUM()
enum class ELoadMethod: uint8
{
	/** Useful for small amounts of Actors. Blocks the game thread during load. */
	LM_Default   UMETA(DisplayName = "Default"),

	/** Useful for medium amounts of Actors with lots of data or components. */
	LM_Deferred  UMETA(DisplayName = "Deferred"),

	/** 
	Useful for large amounts of Actors without many components and data. 
	Try to use deferred loading when possible, since it is more stable.
	*/
	LM_Thread   UMETA(DisplayName = "Multi-Thread"),
};

UENUM()
enum class EFileSaveMethod : uint8
{
	/** Each slot has it's own folder. */
	FM_Desktop  UMETA(DisplayName = "Desktop"),

	/** No folders. No Compression. Each slot has it's own files with '_SlotName' suffix. */
	FM_Console   UMETA(DisplayName = "Console"),
};

UENUM(BlueprintType, meta = (Bitflags))
enum class ESaveTypeFlags : uint8
{
	/** Save Player Controller, Pawn and Player State. */
	SF_Player		= 0		UMETA(DisplayName = "Player Actors"),

	/** Save Level Actors and Level Blueprints. */
	SF_Level		= 1		UMETA(DisplayName = "Level Actors"),
};

UENUM(BlueprintType, meta = (Bitflags))
enum class ELoadTypeFlags : uint8
{
	/** Load Player Controller, Pawn and Player State. */
	LF_Player		= 0		UMETA(DisplayName = "Player Actors"),

	/** Load Level Actors and Level Blueprints. */
	LF_Level		= 1		UMETA(DisplayName = "Level Actors"),
};

UENUM()
enum class EMultiLevelSaveMethod : uint8
{
	/** Multi Level Saving Disabled. */
	ML_Disabled   UMETA(DisplayName = "Disabled"),

	/** For multiple Persistent Levels only. */
	ML_Normal  UMETA(DisplayName = "Basic"),

	/** For one Persistent Level with World Partition or Streaming Sub-Levels. */
	ML_Stream  UMETA(DisplayName = "Streaming"),

	/** For multiple Persistent Levels with World Partition and/or Streaming Sub-Levels. */
	ML_Full  UMETA(DisplayName = "Full"),
};

UENUM()
enum class ECompressionMethod : uint8
{
	/** Use legacy Zlib compression. */
	Zlib,

	/** Use newer Oodle compression. */
	Oodle,
};

UENUM()
enum class EThumbnailImageFormat : uint8
{
	/** Use high quality, but slow png compression. */
	Png,

	/** Use lower quality, but fast jpeg compression. */
	Jpeg,
};


#define ENUM_TO_FLAG(Enum) (1 << static_cast<uint8>(Enum)) 

USTRUCT(BlueprintType)
struct FSaveSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "SaveSlotInfo")
	FString Name;

	UPROPERTY(BlueprintReadOnly, Category = "SaveSlotInfo")
	FDateTime TimeStamp;

	UPROPERTY(BlueprintReadOnly, Category = "SaveSlotInfo")
	FName Level;

	UPROPERTY(BlueprintReadOnly, Category = "SaveSlotInfo")
	TArray<FString> Players;
};

USTRUCT()
struct FComponentSaveData
{
	GENERATED_BODY()

	TArray<uint8> Name;
	FTransform RelativeTransform;
	TArray<uint8> Data;

	friend FArchive& operator<<(FArchive& Ar, FComponentSaveData& ComponentData)
	{
		Ar << ComponentData.Name;
		Ar << ComponentData.RelativeTransform;
		Ar << ComponentData.Data;
		return Ar;
	}
};

USTRUCT()
struct FGameObjectSaveData
{
	GENERATED_BODY()

	TArray<uint8> Data;
	TArray<FComponentSaveData> Components;

	friend FArchive& operator<<(FArchive& Ar, FGameObjectSaveData& GameObjectData)
	{
		Ar << GameObjectData.Data;
		Ar << GameObjectData.Components;
		return Ar;
	}
};

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	TArray<uint8> Class;     
	TArray<uint8> Name;
	FTransform Transform;  
	uint8 Type;
	FGameObjectSaveData SaveData;

	friend FArchive& operator<<(FArchive& Ar, FActorSaveData& ActorData)
	{
		Ar << ActorData.Class;
		Ar << ActorData.Name;
		Ar << ActorData.Transform;
		Ar << ActorData.Type;
		Ar << ActorData.SaveData;
		return Ar;
	}

	FORCEINLINE bool operator ==(const FActorSaveData& A) const
	{
		return A.Name == Name;
	}
};

USTRUCT()
struct FLevelScriptSaveData
{
	GENERATED_BODY()

	FName Name;
	FGameObjectSaveData SaveData;

	friend FArchive& operator<<(FArchive& Ar, FLevelScriptSaveData& ScriptData)
	{
		Ar << ScriptData.Name;
		Ar << ScriptData.SaveData;
		return Ar;
	}

	FORCEINLINE bool operator ==(const FLevelScriptSaveData& A) const
	{
		return A.Name == Name;
	}
};

USTRUCT()
struct FLevelArchive
{
	GENERATED_BODY()

	TArray<FActorSaveData> SavedActors;
	TArray<FLevelScriptSaveData> SavedScripts;
	FGameObjectSaveData SavedGameMode;
	FGameObjectSaveData SavedGameState;
	FName Level;

	friend FArchive& operator<<(FArchive& Ar, FLevelArchive& LevelArchive)
	{
		Ar << LevelArchive.SavedActors;
		Ar << LevelArchive.SavedScripts;
		Ar << LevelArchive.SavedGameMode;
		Ar << LevelArchive.SavedGameState;
		Ar << LevelArchive.Level;
		return Ar;
	}

	FORCEINLINE bool operator ==(const FLevelArchive& A) const
	{
		return A.Level == Level;
	}

	FORCEINLINE bool operator !=(const FLevelArchive& A) const
	{
		return A.Level != Level;
	}

	FORCEINLINE void ReplaceWith(const FLevelArchive& A)
	{
		//Mode is skipped here, since it is always persistent when using level stack.
		SavedActors = A.SavedActors;
		SavedScripts = A.SavedScripts;
		Level = A.Level;
	}

};

USTRUCT()
struct FLevelStackArchive
{
	GENERATED_BODY()

	TArray<FLevelArchive> Archives;

	FGameObjectSaveData SavedGameMode;
	FGameObjectSaveData SavedGameState;

	friend FArchive& operator<<(FArchive& Ar, FLevelStackArchive& StackedArchive)
	{
		Ar << StackedArchive.Archives;
		Ar << StackedArchive.SavedGameMode;
		Ar << StackedArchive.SavedGameState;
		return Ar;
	}

	FORCEINLINE void AddTo(const FLevelArchive& A)
	{
		Archives.Add(A);
	}
};

USTRUCT(BlueprintType)
struct FMultiLevelStreamingData
{
	GENERATED_BODY()

	TArray<FActorSaveData> Actors;
	TArray<FLevelScriptSaveData> Scripts;

	FORCEINLINE void CopyFrom(const FLevelArchive& A)
	{
		Actors = A.SavedActors;
		Scripts = A.SavedScripts;
	}

	template <typename TSaveData, typename TSaveDataArray>
	FORCEINLINE void ReplaceOrAddToArray(const TSaveData& Data, TSaveDataArray& OuputArray)
	{
		//This will replace an existing element or add a new one. 
		const uint32 Index = OuputArray.Find(Data);
		if (Index != INDEX_NONE)
		{
			OuputArray[Index] = Data;
		}
		else
		{
			OuputArray.Add(Data);
		}	
	}

	FORCEINLINE void ReplaceOrAdd(const FLevelArchive& A)
	{
		for (const FActorSaveData& ActorData : A.SavedActors)
		{
			ReplaceOrAddToArray(ActorData, Actors);
		}
		
		for (const FLevelScriptSaveData& ScriptData : A.SavedScripts)
		{
			ReplaceOrAddToArray(ScriptData, Scripts);
		}		
	}

	FORCEINLINE void PruneActors()
	{
		//This prevents respawning already destroyed runtime Actors from previous save data.
		const TArray<FActorSaveData> PrunedActors = Actors;
		for (const FActorSaveData& ActorData : PrunedActors)
		{
			if (ActorData.Type == uint8(EActorType::AT_Runtime))
			{
				Actors.Remove(ActorData);
			}
		}
	}
};

USTRUCT()
struct FPawnSaveData
{
	GENERATED_BODY()

	FVector Position;
	FRotator Rotation;
	FGameObjectSaveData SaveData;

	friend FArchive& operator<<(FArchive& Ar, FPawnSaveData& PawnData)
	{
		Ar << PawnData.Position;
		Ar << PawnData.Rotation;
		Ar << PawnData.SaveData;
		return Ar;
	}
};

USTRUCT()
struct FControllerSaveData
{
	GENERATED_BODY()

	FRotator Rotation;
	FGameObjectSaveData SaveData;

	friend FArchive& operator<<(FArchive& Ar, FControllerSaveData& ControllerData)
	{
		Ar << ControllerData.Rotation;
		Ar << ControllerData.SaveData;
		return Ar;
	}
};

USTRUCT()
struct FPlayerArchive
{
	GENERATED_BODY()

	FControllerSaveData SavedController;
	FPawnSaveData SavedPawn;
	FGameObjectSaveData SavedPlayerState;
	FName Level;

	friend FArchive& operator<<(FArchive& Ar, FPlayerArchive& PlayerArchive)
	{
		Ar << PlayerArchive.SavedController;
		Ar << PlayerArchive.SavedPawn;
		Ar << PlayerArchive.SavedPlayerState;
		Ar << PlayerArchive.Level;
		return Ar;
	}
};

USTRUCT()
struct FPlayerPositionArchive
{
	GENERATED_BODY()

	FVector Position;
	FRotator Rotation;
	FRotator ControlRotation;

	friend FArchive& operator<<(FArchive& Ar, FPlayerPositionArchive& PosArchive)
	{
		Ar << PosArchive.Position;
		Ar << PosArchive.Rotation;
		Ar << PosArchive.ControlRotation;
		return Ar;
	}

	FORCEINLINE void SetFromPlayerArchive(const FPlayerArchive& A)
	{
		Position = A.SavedPawn.Position;
		Rotation = A.SavedPawn.Rotation;
		ControlRotation = A.SavedController.Rotation;
	}
};

USTRUCT()
struct FPlayerStackArchive
{
	GENERATED_BODY()

	FPlayerArchive PlayerArchive;
	TMap<FName, FPlayerPositionArchive> LevelPositions;

	friend FArchive& operator<<(FArchive& Ar, FPlayerStackArchive& StackedArchive)
	{
		//Level from PlayerArchive is obsolete in this case.
		Ar << StackedArchive.PlayerArchive;
		Ar << StackedArchive.LevelPositions;
		return Ar;
	}

	FORCEINLINE void ReplaceOrAdd(const FPlayerArchive& A)
	{
		PlayerArchive = A;

		FPlayerPositionArchive NewPos;
		NewPos.SetFromPlayerArchive(A);

		LevelPositions.Add(A.Level, NewPos);
	}

	FORCEINLINE bool IsEmpty()
	{
		return EMS::ArrayEmpty(LevelPositions);
	}

	FORCEINLINE bool HasZeroPositions()
	{
		return IsEmpty();
	}
};

struct FSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FSaveGameArchive(FArchive& InInnerArchive) : FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		ArIsSaveGame = true; //Requires structs to be prepared for serialization(See SerializeStructProperties)
		ArNoDelta = true;  //Allow to save default values
	}
};




