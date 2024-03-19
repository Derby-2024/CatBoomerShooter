//Easy Multi Save - Copyright (C) 2024 by Michael Hegemann.  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EMSCustomSaveGame.generated.h"


UCLASS()
class EASYMULTISAVE_API UEMSCustomSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	/** The name of the custom save file. If empty it will use the class name. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Files")
	FString SaveGameName;
	
	/** If you want to use the current save slot for the custom save file. */
	UPROPERTY(EditDefaultsOnly, Category = "Files")
	bool bUseSaveSlot;
};
