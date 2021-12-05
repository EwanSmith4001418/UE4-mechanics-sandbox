// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MOV_ScriptDoor.generated.h"

//Forward Declaration
class UBoxComponent;

UCLASS()
class MYPROJECT_API AMOV_ScriptDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMOV_ScriptDoor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Open;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* Close;

	FRotator ClosedRotation;
	FRotator DesiredRotation;
	FRotator OpenRotation;
	bool OnOpen;
	bool OnClose;
};