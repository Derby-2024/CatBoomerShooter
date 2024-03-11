// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealthAnyDamageSignature, float, Damage, float, CurrentHealth, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATBOOMERSHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1", UIMin = "1"))
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void DamageApplication(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

	UFUNCTION(BlueprintNativeEvent)
	void HealthDestroyed(class AController* FinalInstigator, AActor* FinalDamageCauser);

public:

	UPROPERTY(BlueprintAssignable)
	FHealthAnyDamageSignature OnTakeAnyDamage;
};
