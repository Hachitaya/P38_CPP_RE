// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GamePlayStatics.h"
#include "MyRocket.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("StaticMesh'/Game/New_P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("StaticMesh'/Game/New_P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
		Right->SetRelativeLocation(FVector(37.f,21.f,0.4f));
		Left->SetRelativeLocation(FVector(37.f, -21.f, 0.4f));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 2;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(80.f, 0, 0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = MoveSpeed;

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Left->AddLocalRotation(FRotator(0, 0, PropellerRotation * DeltaTime));
	Right->AddLocalRotation(FRotator(0, 0, PropellerRotation * DeltaTime));

	AddMovementInput(GetActorForwardVector());
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyPawn::Yaw);
}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0, 0));
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0, 0, Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

void AMyPawn::Yaw(float Value)
{
	AddActorLocalRotation(FRotator(0, Value * RotationSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0));
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AActor>(AMyRocket::StaticClass(), Arrow->K2_GetComponentLocation(), Arrow->K2_GetComponentRotation());
}