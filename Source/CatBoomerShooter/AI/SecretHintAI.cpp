// Fill out your copyright notice in the Description page of Project Settings.


#include "SecretHintAI.h"

/* Spawns, remains stationary until player comes within a certain distance of it, while player maintains this distance it will make progress across a predetermined path
nodes will be created along this path that the ai will stop at until the player comes next to it. - 2 different ways of progressing seems like. Upon reaching its final destination
it will destroy itself. Also check if it wants to respawn after the secret has been collected in the past for replayability. 
Potenial ways of making - Have an array that takes in coordinates and cycles through them. - Not good for reuseability and precision. Maybe viable depending on how many are required. 
Consider whether grounded or floating entity. 
variables
movement speed, animstate, playerdistance, movementnodes, bool collected (either doesn't spawn or lowers alpha), 

While (playerdistance <= activeMoveDist) 
{	
	hintMoveSpeed = 5.0f  //setup public in h
	movetonode(nodearray(y)); - need to test is calling moveto waits until it reaches point before being recalled or not, assume not and will have to buffer. 
	if hintpos == nodearray(y)
	{
		y++ ;
	}
		if y == nodearray.getlength +1
		{
			GetOwner->Destroy();
		}
}

While (playerdistance != activeMoveDist)
	hintMoveSpeed = 0.f;
Animations: Spawn animation, animation when reaching a node?, despawn animation, idle animation, movement animation. 
*/ 




// Sets default values
ASecretHintAI::ASecretHintAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASecretHintAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecretHintAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*while (playerdistance <= activeMoveDist)
	{
		hintMoveSpeed = 5.0f  //setup public in h
			movetonode(nodearray(y)); //need to test is calling moveto waits until it reaches point before being recalled or not, assume not and will have to buffer.
			if hintpos == nodearray(y)
			{
				y++;
			}
		if y == nodearray.getlength + 1
		{
			GetOwner->Destroy();
		}
	}

	While(playerdistance != activeMoveDist)
		hintMoveSpeed = 0.f; */
}

// Called to bind functionality to input
void ASecretHintAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

