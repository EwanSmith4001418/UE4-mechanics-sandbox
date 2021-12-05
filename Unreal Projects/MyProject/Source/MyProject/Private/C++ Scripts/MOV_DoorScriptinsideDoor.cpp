// Fill out your copyright notice in the Description page of Project Settings.


#include "C++ Scripts/MOV_DoorScriptinsideDoor.h"

// Sets default values for this component's properties
UMOV_DoorScriptinsideDoor::UMOV_DoorScriptinsideDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMOV_DoorScriptinsideDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMOV_DoorScriptinsideDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

