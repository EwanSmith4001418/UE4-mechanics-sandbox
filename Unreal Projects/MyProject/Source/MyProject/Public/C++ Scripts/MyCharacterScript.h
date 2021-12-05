// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacterScript.generated.h"

class UBoxComponent;
class UCapsuleComponent;

UCLASS()
class MYPROJECT_API AMyCharacterScript : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacterScript();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void HorizontalMove(float value);
	void VerticalMove(float value);

	void HorizontalRot(float value);
	void VerticalRot(float value);

	void CheckJump();

	void Zoom(float value);
	void Switch();

	void Teleport();
	void rightClick();

	UPROPERTY()
		bool jumping;
	UPROPERTY()
		bool firstPerson;
	UPROPERTY()
		bool canTeleport;
	bool rightMB;
	UPROPERTY(VisibleAnywhere)
		float chargeTime;
	UPROPERTY()
		FVector teleport;

	UPROPERTY()
		UCameraComponent* Cam;
	UPROPERTY()
		USpringArmComponent* Arm;
	UPROPERTY()
		AActor* Teleporter;
	UPROPERTY(EditAnywhere)
		UBoxComponent* TeleportTarget;
};
