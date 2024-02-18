// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWhip.h"
#include "Kismet/GameplayStatics.h"
#include "CatBoomerShooter/Character/BasePlayerCharacter.h"
#include "CatBoomerShooter/Character/BasePlayerInterface.h"


// Sets default values
ABaseWhip::ABaseWhip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>("Default Scene Component");
	RootComponent = DefaultSceneComponent;

	Whip_SK = CreateDefaultSubobject<USkeletalMeshComponent>("Whip Skeletal Mesh");
	Whip_SK->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);

	AttackOneCollision = CreateDefaultSubobject<UBoxComponent>("Attack One Collider");
	AttackOneCollision->SetupAttachment(RootComponent);

	AttackTwoCollision = CreateDefaultSubobject<UBoxComponent>("Attack Two Collider");
	AttackTwoCollision->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABaseWhip::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	USceneComponent* WhipLocation = IBasePlayerInterface::Execute_GetPlayerWhipLocation(PlayerCharacter);
	
	if(IsValid(WhipLocation))
	{
		this->AttachToComponent(WhipLocation, FAttachmentTransformRules::SnapToTargetIncludingScale, "GripPoint");
		this->SetActorHiddenInGame(true);
	}
	
}

// Called every frame
void ABaseWhip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWhip::Attack()
{
	if(isAttacking)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	isAttacking = true;
	this->SetActorHiddenInGame(false);
}
