// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMyRocket::AMyRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	RocketBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketBody"));
	RocketBody->SetupAttachment(Box);
	RocketBody->SetRelativeRotation(FRotator(-90, 0, 0));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_RocketBody(TEXT("StaticMesh'/Game/New_P38/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_RocketBody.Succeeded())
	{
		RocketBody->SetStaticMesh(SM_RocketBody.Object);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 6000.f;
	Movement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(2.f);
	
}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

