// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogHealth);

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

	if (ResetHealthOnBeginplay)
		Health = MaxHealth;
	else
		Health = FMath::Clamp(Health, 0, MaxHealth);
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageApplication);
}

void UHealthComponent::DamageApplication_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser)
{
	if (Invincible) return;

	if (Damage <= 0.f) return;

	Health -= Damage;
	UE_LOG(LogHealth, Log, TEXT("%s Health: %f"), *UKismetSystemLibrary::GetDisplayName(DamagedActor), Health);

	OnTakeAnyDamage.Broadcast(Damage, Health, Instigator, DamageCauser);

	if (Health <= 0.f)
	{
		UE_LOG(LogHealth, Log, TEXT("%s Health depleated"), *UKismetSystemLibrary::GetDisplayName(DamagedActor));
		OnKill(Instigator, DamageCauser);
	}
	if (Health > MaxHealth) Health = MaxHealth;
}

void UHealthComponent::OnKill_Implementation(class AController* FinalInstigator, AActor* FinalDamageCauser)
{
	GetOwner()->Destroy();
}
