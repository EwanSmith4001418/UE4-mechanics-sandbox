// Fill out your copyright notice in the Description page of Project Settings.


#include "C++ Scripts/MOV_ScriptLift.h"

// Other include
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AMOV_ScriptLift::AMOV_ScriptLift()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("MyRoot"));
	RootComponent = SceneComp;

	Lift = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lift"));
	Lift->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Up = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision2"));
	Up->AttachToComponent(Lift, FAttachmentTransformRules::KeepWorldTransform);

	Down = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision3"));
	Down->AttachToComponent(Lift, FAttachmentTransformRules::KeepWorldTransform);

	Up->OnComponentBeginOverlap.AddDynamic(this, &AMOV_ScriptLift::OnBoxBeginOverlap);
	Up->OnComponentEndOverlap.AddDynamic(this, &AMOV_ScriptLift::OnBoxEndOverlap);

	Down->OnComponentBeginOverlap.AddDynamic(this, &AMOV_ScriptLift::OnBoxBeginOverlap);
	Down->OnComponentEndOverlap.AddDynamic(this, &AMOV_ScriptLift::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void AMOV_ScriptLift::BeginPlay()
{
	Super::BeginPlay();
	//Setup Location
	Elevation = Lift->GetComponentLocation();
	DesiredElevation = Lift->GetComponentLocation();
	UpElevation = 100.f;
	DownElevation = -100.f;
}

// Called every frame
void AMOV_ScriptLift::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (OnUp)
	{
		DesiredElevation.Z += UpElevation * DeltaTime;
	}
	if (OnDown)
	{
		DesiredElevation.Z += DownElevation * DeltaTime;
	}
	//UE_LOG(LogTemp, Warning, FString::Printf(TEXT(":",playerCharacter.ToString()));	
	Elevation = Lift->GetComponentLocation();
	if (!Elevation.Equals(DesiredElevation, 1.0f))
	{
		const FVector NewElevation = FMath::Lerp(Elevation, DesiredElevation, 0.01f);
		Lift->SetWorldLocation(NewElevation);
	}
	
}


void AMOV_ScriptLift::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision Detected"));
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		if (OverlappedComp->GetUniqueID() == Up->GetUniqueID())
		{
			OnUp = true;
		}
		else if (OverlappedComp->GetUniqueID() == Down->GetUniqueID())
		{
			OnDown = true;
		}
	}
	

}

void AMOV_ScriptLift::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		OnUp = false;
		OnDown = false;
		DesiredElevation.Z -= DesiredElevation.Z - Elevation.Z;
	}
}