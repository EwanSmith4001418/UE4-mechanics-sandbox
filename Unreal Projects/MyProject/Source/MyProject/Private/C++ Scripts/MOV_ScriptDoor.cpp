// Fill out your copyright notice in the Description page of Project Settings.

// This fields header
#include "C++ Scripts/MOV_ScriptDoor.h"

// Other include
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AMOV_ScriptDoor::AMOV_ScriptDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = SceneComp;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Open = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision2"));
	Open->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Close = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision3"));
	Close->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Open->OnComponentBeginOverlap.AddDynamic(this, &AMOV_ScriptDoor::OnBoxBeginOverlap);
	Open->OnComponentEndOverlap.AddDynamic(this, &AMOV_ScriptDoor::OnBoxEndOverlap);

	Close->OnComponentBeginOverlap.AddDynamic(this, &AMOV_ScriptDoor::OnBoxBeginOverlap);
	Close->OnComponentEndOverlap.AddDynamic(this, &AMOV_ScriptDoor::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void AMOV_ScriptDoor::BeginPlay()
{
	Super::BeginPlay();
	//Setup rotators
	ClosedRotation = Door->GetComponentRotation();
	DesiredRotation = Door->GetComponentRotation();
	OpenRotation = ClosedRotation + FRotator(0.0f, -90.0f, 0.0f);

}

// Called every frame
void AMOV_ScriptDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (OnOpen)
	{
		DesiredElevation.Z += UpElevation * DeltaTime;
	}
	if (OnClose)
	{
		DesiredElevation.Z += DownElevation * DeltaTime;
	} */

	const FRotator CurrentRotation = Door->GetComponentRotation();
	if (!CurrentRotation.Equals(DesiredRotation, 1.0f))
	{
		const FRotator NewRotation = FMath::Lerp(CurrentRotation, DesiredRotation, 0.01f);
		Door->SetWorldRotation(NewRotation);
	}
}


void AMOV_ScriptDoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("COllision Detected"));
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		//DesiredRotation = OpenRotation;

		if (OverlappedComp->GetUniqueID() == Open->GetUniqueID())
		{
			
			DesiredRotation = FRotator(-OpenRotation.Pitch, -OpenRotation.Yaw, -OpenRotation.Roll);
		}
		else if (OverlappedComp->GetUniqueID() == Close->GetUniqueID())
		{
			DesiredRotation = OpenRotation;
		}

	}
}

void AMOV_ScriptDoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		DesiredRotation = ClosedRotation;
	}
}