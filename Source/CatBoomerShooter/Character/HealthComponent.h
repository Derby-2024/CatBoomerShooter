// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATBOOMERSHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	int GetHealth() { return Health; }
	int SetHealth(int HealthSet) { Health = HealthSet; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;
	float Health = 0.f;
	bool Alive;

	UFUNCTION()
	void DamageApplication(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);
	uint8 bCanEverTick : 1;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetAlive() { return Alive; }
	void SetAlive(bool isAlive) { Alive = isAlive; }
		
};
