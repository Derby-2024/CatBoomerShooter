//Easy Multi Save - Copyright (C) 2024 by Michael Hegemann.  

#include "EMSAsyncSaveGame.h"
#include "EMSFunctionLibrary.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "Async/Async.h"

/**
Init
**/

UEMSAsyncSaveGame::UEMSAsyncSaveGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsActive = false;
	bFinishedStep = false;
	bHasFailed = false;
}

ESaveGameMode UEMSAsyncSaveGame::GetMode(int32 Data)
{
	if (Data & ENUM_TO_FLAG(ESaveTypeFlags::SF_Player))
	{
		if (Data & ENUM_TO_FLAG(ESaveTypeFlags::SF_Level))
		{
			return ESaveGameMode::MODE_All;
		}
		else
		{
			return ESaveGameMode::MODE_Player;
		}
	}

	return ESaveGameMode::MODE_Level;
}

UEMSAsyncSaveGame* UEMSAsyncSaveGame::AsyncSaveActors(UObject* WorldContextObject, int32 Data)
{
	if (UEMSObject* EMSObject = UEMSObject::Get(WorldContextObject))
	{
		if (!EMSObject->IsAsyncSaveOrLoadTaskActive(GetMode(Data)))
		{
			UEMSAsyncSaveGame* SaveTask = NewObject<UEMSAsyncSaveGame>(GetTransientPackage());
			SaveTask->WorldContext = WorldContextObject;
			SaveTask->Data = Data;
			SaveTask->Mode = GetMode(Data);
			SaveTask->EMS = EMSObject;

			return SaveTask;
		}
	}

	return nullptr;
}

bool UEMSAsyncSaveGame::NativeSpawnSaveTask(UEMSObject* EMSObject, const int32& Data)
{
	UEMSAsyncSaveGame* SaveTask = NewObject<UEMSAsyncSaveGame>(GetTransientPackage());
	if (SaveTask)
	{
		SaveTask->WorldContext = EMSObject;
		SaveTask->Data = Data;
		SaveTask->Mode = GetMode(Data);
		SaveTask->EMS = EMSObject;
		SaveTask->Activate();

		return true;
	}

	return false;
}

bool UEMSAsyncSaveGame::NativeSaveLevelActors(UEMSObject* EMSObject)
{
	return NativeSpawnSaveTask(EMSObject, ENUM_TO_FLAG(ESaveTypeFlags::SF_Level));
}

void UEMSAsyncSaveGame::Activate()
{
	bIsActive = true;

	if (EMS)
	{
		EMS->PrepareLoadAndSaveActors(Data, EAsyncCheckType::CT_Save, false);

		EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncSaveGame::StartSaving);
	}
}

void UEMSAsyncSaveGame::StartSaving()
{
	bHasFailed = false;

	if (EMS)
	{
		//Always save slot
		EMS->SaveSlotInfoObject();

		EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncSaveGame::SavePlayer);
	}
}

/**
Player
**/

void UEMSAsyncSaveGame::SavePlayer()
{
	bFinishedStep = false;

	if (EMS)
	{
		if (UEMSPluginSettings::Get()->bMultiThreadSaving && FPlatformProcess::SupportsMultithreading())
		{
			AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [this]()
			{
				InternalSavePlayer();
			});
		}
		else
		{
			InternalSavePlayer();
		}

		TryMoveToNextStep(ENextStepType::SaveLevel);
	}
}

void UEMSAsyncSaveGame::InternalSavePlayer()
{
	if (Data & ENUM_TO_FLAG(ESaveTypeFlags::SF_Player))
	{
		if (!EMS->SavePlayerActors())
		{
			bHasFailed = true;
		}
	}

	bFinishedStep = true;
}

/**
Level
**/

void UEMSAsyncSaveGame::SaveLevel()
{
	bFinishedStep = false;

	if (EMS)
	{
		if (UEMSPluginSettings::Get()->bMultiThreadSaving && FPlatformProcess::SupportsMultithreading())
		{
			AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [this]()
			{
				InternalSaveLevel();
			});
		}
		else
		{
			InternalSaveLevel();
		}

		TryMoveToNextStep(ENextStepType::FinishSave);
	}
}

void UEMSAsyncSaveGame::InternalSaveLevel()
{
	if (Data & ENUM_TO_FLAG(ESaveTypeFlags::SF_Level))
	{
		const bool bPrevHasFailed = bHasFailed;

		if(EMS->SaveLevelActors())
		{
			bHasFailed = false;
		}
		else
		{
			bHasFailed = bPrevHasFailed;
		}
	}

	bFinishedStep = true;
}

/**
Finish
**/

void UEMSAsyncSaveGame::FinishSaving()
{
	if (EMS)
	{
		bIsActive = false;

		EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncSaveGame::CompleteSavingTask);
	}
}

void UEMSAsyncSaveGame::CompleteSavingTask()
{
	if (bHasFailed)
	{
		OnFailed.Broadcast();
	}
	else
	{
		OnCompleted.Broadcast();
	}

	SetReadyToDestroy();
}

/**
Helper Functions
**/

void UEMSAsyncSaveGame::TryMoveToNextStep(ENextStepType Step)
{
	//This is used to delay further execution until multi-thread code finished, but without blocking.

	if (EMS)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, Step]()
		{
			if (bFinishedStep)
			{
				if (Step == ENextStepType::FinishSave)
				{
					EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncSaveGame::FinishSaving);
				}
				else
				{
					EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncSaveGame::SaveLevel);
				}
			}
			else
			{
				TryMoveToNextStep(Step);
			}
		});

		EMS->GetTimerManager().SetTimerForNextTick(TimerDelegate);
	}
}