//Easy Multi Save - Copyright (C) 2024 by Michael Hegemann.  

#include "EMSAsyncLoadGame.h"
#include "EMSFunctionLibrary.h"
#include "EMSObject.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectIterator.h"
#include "Engine/Engine.h"


/**
Init
**/

UEMSAsyncLoadGame::UEMSAsyncLoadGame(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsActive = false;
	bLoadFailed = false;
	bFullReload = false;
	bDeferredLoadSuccess = false;
}

ESaveGameMode UEMSAsyncLoadGame::GetMode(int32 Data)
{
	if (Data & ENUM_TO_FLAG(ELoadTypeFlags::LF_Player))
	{
		if (Data & ENUM_TO_FLAG(ELoadTypeFlags::LF_Level))
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

UEMSAsyncLoadGame* UEMSAsyncLoadGame::AsyncLoadActors(UObject* WorldContextObject, int32 Data, bool bFullReload)
{
	if (UEMSObject* EMSObject = UEMSObject::Get(WorldContextObject))
	{
		if (!EMSObject->IsAsyncSaveOrLoadTaskActive(GetMode(Data)))
		{
			UEMSAsyncLoadGame* LoadTask = NewObject<UEMSAsyncLoadGame>(GetTransientPackage());		
			LoadTask->WorldContext = WorldContextObject;
			LoadTask->Data = Data;
			LoadTask->Mode = GetMode(Data);
			LoadTask->bFullReload = bFullReload;
			LoadTask->EMS = EMSObject;

			return LoadTask;
		}
	}

	return nullptr;
}

bool UEMSAsyncLoadGame::NativeSpawnLoadTask(UEMSObject* EMSObject, const int32& Data, const bool& bFullReload)
{
	UEMSAsyncLoadGame* LoadTask = NewObject<UEMSAsyncLoadGame>(GetTransientPackage());
	if (LoadTask)
	{
		LoadTask->WorldContext = EMSObject;
		LoadTask->Data = Data;
		LoadTask->Mode = GetMode(Data);
		LoadTask->bFullReload = bFullReload;
		LoadTask->EMS = EMSObject;
		LoadTask->Activate();

		return true;
	}

	return false;
}

bool UEMSAsyncLoadGame::NativeLoadLevelActors(UEMSObject* EMSObject)
{
	return UEMSAsyncLoadGame::NativeSpawnLoadTask(EMSObject, ENUM_TO_FLAG(ELoadTypeFlags::LF_Level), false);
}

void UEMSAsyncLoadGame::Activate()
{
	bIsActive = true;
	EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::PreLoading);
}

/**
Load Actors
**/

void UEMSAsyncLoadGame::PreLoading()
{
	if (EMS)
	{
		if (Data & ENUM_TO_FLAG(ELoadTypeFlags::LF_Player))
		{
			if (!EMS->HasValidPlayer())
			{
				EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::PreLoading);
				return;
			}
		}

		if (Data & ENUM_TO_FLAG(ELoadTypeFlags::LF_Level))
		{
			if (!EMS->HasValidGameMode())
			{
				EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::PreLoading);
				return;
			}
		}

		EMS->PrepareLoadAndSaveActors(Data, EAsyncCheckType::CT_Load, bFullReload);

		EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::StartLoading);

		const float WaitTime = UEMSPluginSettings::Get()->AsyncWaitDelay;
		if (WaitTime > 0.f)
		{
			EMS->GetTimerManager().SetTimer(FailedTimerHandle, FTimerDelegate::CreateUObject(this, &UEMSAsyncLoadGame::FailLoadingTask), WaitTime, false);
		}
	}
}

void UEMSAsyncLoadGame::ClearFailTimer()
{
	if (EMS)
	{
		if (FailedTimerHandle.IsValid())
		{
			EMS->GetTimerManager().ClearTimer(FailedTimerHandle);
		}
	}
}

void UEMSAsyncLoadGame::StartLoading()
{
	if (EMS)
	{
		EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::LoadPlayer);
	}
}

void UEMSAsyncLoadGame::LoadPlayer()
{
	bLoadFailed = true;

	if (EMS)
	{
		if (Data & ENUM_TO_FLAG(ELoadTypeFlags::LF_Player))
		{
			if (EMS->TryLoadPlayerFile())
			{
				SetLoadNotFailed();

				EMS->LoadPlayerActors(this);	
			}
		}

		EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::LoadLevel);
	}
}

void UEMSAsyncLoadGame::LoadLevel()
{ 
	if (EMS)
	{
		if (Data & ENUM_TO_FLAG(ELoadTypeFlags::LF_Level))
		{
			if (EMS->TryLoadLevelFile())
			{
				SetLoadNotFailed();

				EMS->LoadLevelActors(this);
			}
			else
			{
				FinishLoading();
			}
		}
		else
		{
			FinishLoading();
		}
	}
}

/**
Finish
**/

void UEMSAsyncLoadGame::FinishLoading()
{
	if (EMS)
	{
		//Has to be a tick before broadcast.
		bIsActive = false;

		ClearFailTimer();

		if (bLoadFailed)
		{
			EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::FailLoadingTask);
		}
		else
		{
			EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::CompleteLoadingTask);
		}
	}
}

void UEMSAsyncLoadGame::CompleteLoadingTask()
{
	OnCompleted.Broadcast();
	SetReadyToDestroy();
}

void UEMSAsyncLoadGame::FailLoadingTask()
{
	OnFailed.Broadcast();
	SetReadyToDestroy();
}

/**
Deferred Loading
**/

void UEMSAsyncLoadGame::StartDeferredLoad()
{
	if (EMS)
	{
		SavedActors = EMS->SavedActors;
		DeferredLoadActors();
	}
}

void UEMSAsyncLoadGame::DeferredLoadActors()
{
	if (EMS)
	{
		const uint32 Batch = UEMSPluginSettings::Get()->DeferredLoadStackSize;  
		uint32 LoadNum = 0;

		if (LoadedActorNum < SavedActors.Num())
		{
			while (LoadNum < Batch)
			{
				const int32 FullBatch = LoadNum + LoadedActorNum;
				if (FullBatch >= SavedActors.Num())
				{
					break;
				}

				const uint32 MinLoadNum = FMath::Min(FullBatch, SavedActors.Num());
				bDeferredLoadSuccess = EMS->SpawnOrUpdateLevelActor(SavedActors[MinLoadNum]);

				LoadNum++;
			}

			LoadedActorNum += Batch;

			EMS->GetTimerManager().SetTimerForNextTick(this, &UEMSAsyncLoadGame::DeferredLoadActors);
		}
		else
		{
			if (bDeferredLoadSuccess)
			{
				EMS->LogFinishLoadingLevel();
			}

			FinishLoading();
		}
	}
}


