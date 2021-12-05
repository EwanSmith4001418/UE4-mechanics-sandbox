// Fill out your copyright notice in the Description page of Project Settings.


#include "C++ Scripts/MOV_DestructibleWall.h"

// Sets default values
AMOV_DestructibleWall::AMOV_DestructibleWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMOV_DestructibleWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMOV_DestructibleWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

