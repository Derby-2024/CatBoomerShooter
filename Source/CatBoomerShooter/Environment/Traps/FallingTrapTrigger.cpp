// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingTrapTrigger.h"

AFallingTrapTrigger::AFallingTrapTrigger()
{
    OnActorBeginOverlap.AddDynamic(this, &AFallingTrapTrigger::Event);
}

void AFallingTrapTrigger:: BeginPlay()
{
    Super::BeginPlay();
}

void AFallingTrapTrigger::Event(class AActor* overlappedActor, class AActor* otherActor)
{
    //check is there is an other actor and the other actor isnt this actor
    if(otherActor && otherActor !=this)
    {
        if(FallingObject)
        {
            FallingObject->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
        }
    }
}