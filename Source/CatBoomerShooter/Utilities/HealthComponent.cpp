// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent() : Super()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//Health = FMath::Clamp(Health, 0, MaxHealth);
	Health = MaxHealth;
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageApplication);
}

void UHealthComponent::DamageApplication_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;

	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health)

	if (Health <= 0.f)
	{
		GetOwner()-> Destroy();
	}
	if (Health > MaxHealth) Health = MaxHealth;
}
