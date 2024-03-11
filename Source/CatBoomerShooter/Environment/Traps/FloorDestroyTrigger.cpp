// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorDestroyTrigger.h"


AFloorDestroyTrigger::AFloorDestroyTrigger()
{
    OnActorBeginOverlap.AddDynamic(this, &AFloorDestroyTrigger::Event);
}

void AFloorDestroyTrigger:: BeginPlay()
{
    Super::BeginPlay();
}

void AFloorDestroyTrigger::Event(class AActor* overlappedActor, class AActor* otherActor)
{
    //check is there is an other actor and the other actor isnt this actor
    if(otherActor && otherActor !=this)
    {
        if(DestroyObject)
        {
            DestroyObject->Destroy();
        }
    }
}
