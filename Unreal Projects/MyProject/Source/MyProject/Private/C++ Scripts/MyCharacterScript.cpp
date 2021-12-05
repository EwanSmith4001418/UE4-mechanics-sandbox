// Fill out your copyright notice in the Description page of Project Settings.


#include "C++ Scripts/MyCharacterScript.h"

//Other includes
#include "Components/BoxComponent.h"

// Sets default values
AMyCharacterScript::AMyCharacterScript()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Arm->AttachTo(RootComponent);
	Arm->TargetArmLength = 300.0f;
	Arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));


	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Cam->AttachTo(RootComponent);
	Cam->AttachTo(Arm,USpringArmComponent::SocketName);
	//Cam->SetRelativeLocation(FVector(0, 0, 40));

	TeleportTarget = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportTarget"));
	TeleportTarget->AttachTo(RootComponent);

	jumping = false;

	firstPerson = false;

	canTeleport = true;
	rightMB = false;
	chargeTime = 0.0f;

	//Teleporter = CreateDefaultSubobject<UBoxComponent>(TEXT("Teleporter"));
	//Teleporter->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMyCharacterScript::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacterScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (jumping)
	{
		Jump();
	}
	/*if (firstPerson == false)
	{
		canTeleport = false;
	}
	*/
	//else {
		if (canTeleport == false)
		{
			chargeTime += DeltaTime;
			if (chargeTime >= 3)
			{
				canTeleport = true;
			}
		}
	//}
		if (canTeleport && rightMB)
		{
			FVector fromTeleport = Cam->GetComponentLocation();
			FVector toTeleport = Cam->GetForwardVector();
			fromTeleport = FVector(fromTeleport.X + (toTeleport.X * 50), fromTeleport.Y + (toTeleport.Y * 50), fromTeleport.Z + (toTeleport.Z * 50));
			FVector endTeleport = fromTeleport + (toTeleport * 1000);
			FHitResult hit;
			float dist = 50;
			if (GetWorld())
			{
				bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, fromTeleport, endTeleport, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
				//DrawDebugLine(GetWorld(), fromTeleport, endTeleport, FColor::Green, false, 2.f, 0.f, 10.f);
				if (actorHit && hit.GetActor())
				{
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetFName().ToString());
					Teleporter = hit.GetActor();
					//FVector playerDist = Teleporter->GetActorLocation() - hit.ImpactPoint;
					TeleportTarget->SetWorldLocation(FVector(hit.ImpactPoint.X - (toTeleport.X * dist), hit.ImpactPoint.Y - (toTeleport.Y * dist), hit.ImpactPoint.Z - (toTeleport.Z * dist*1.5)));
					teleport = TeleportTarget->GetComponentLocation();
				}
				else
				{
					TeleportTarget->SetWorldLocation(FVector(endTeleport.X, endTeleport.Y, endTeleport.Z));
					teleport = TeleportTarget->GetComponentLocation();
				}
			}

		}
}

// Called to bind functionality to input
void AMyCharacterScript::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Horizontal", this, &AMyCharacterScript::HorizontalMove);
	InputComponent->BindAxis("Vertical", this, &AMyCharacterScript::VerticalMove);

	InputComponent->BindAction("Jump", IE_Pressed,this, &AMyCharacterScript::CheckJump);
	InputComponent->BindAction("Jump", IE_Released,this, &AMyCharacterScript::CheckJump);

	InputComponent->BindAxis("Zoom",this, &AMyCharacterScript::Zoom);
	InputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AMyCharacterScript::Switch);

	InputComponent->BindAction("Teleport", IE_Pressed, this, &AMyCharacterScript::Teleport);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AMyCharacterScript::rightClick);
	InputComponent->BindAction("RightClick", IE_Released, this, &AMyCharacterScript::rightClick);

	InputComponent->BindAxis("HorizontalRotation", this, &AMyCharacterScript::HorizontalRot);
	InputComponent->BindAxis("VerticalRotation", this, &AMyCharacterScript::VerticalRot);
}

void AMyCharacterScript::HorizontalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AMyCharacterScript::VerticalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AMyCharacterScript::HorizontalRot(float value)
{
	if (value)
	{
		AddActorLocalRotation(FRotator(0, value ,0));
	}
}

void AMyCharacterScript::VerticalRot(float value)
{
	if (value)
	{
		float temp = 0;
		if (firstPerson)
		{
			temp = Cam->GetRelativeRotation().Pitch + value;
			if (temp < 90 && temp > -80)
			{
				Cam->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
		else 
		{
			temp = Arm->GetRelativeRotation().Pitch + value;
			if (temp < 90 && temp > -80)
			{
				Arm->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
		
	}
}

void AMyCharacterScript::CheckJump()
{
	if (jumping)
	{
		jumping = false;
	}
	else
	{
		jumping = true;
	}
}

void AMyCharacterScript::Zoom(float value)
{
	if (value)
	{
		float temp = Arm->TargetArmLength + (value * -10);

		if (temp < 310 && temp > 140)
		{
			Arm->TargetArmLength = temp;
		}
	}
}

void AMyCharacterScript::Switch()
{
	if (firstPerson)
	{
		Arm->TargetArmLength = 300.f;
		Arm->SetRelativeRotation(FRotator(-45, 0, 0));
		Cam->SetRelativeRotation(FRotator(0, 0, 0));
		Cam->AttachTo(Arm, USpringArmComponent::SocketName);
		firstPerson = false;
	}
	else
	{
		Cam->AttachTo(RootComponent);
		Cam->SetRelativeLocation(FVector(0, 0, 40));
		firstPerson = true;
	}
}

void AMyCharacterScript::rightClick()
{
	rightMB = !rightMB;
}
void AMyCharacterScript::Teleport()
{
	chargeTime = 0;
	RootComponent->SetRelativeLocation(teleport);
}