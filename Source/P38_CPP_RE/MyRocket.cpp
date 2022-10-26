// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
#include "Materials/MaterialInstance.h"


// Sets default values
AMyRocket::AMyRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetCollisionProfileName(TEXT("Tirgger"));
	Box->SetGenerateOverlapEvents(true);
	//Collision
	Box->OnComponentBeginOverlap.AddDynamic(this, &AMyRocket::OnOverlapBegin);

	RocketBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketBody"));
	RocketBody->SetupAttachment(Box);
	RocketBody->SetRelativeRotation(FRotator(-90, 0, 0));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_RocketBody(TEXT("StaticMesh'/Game/New_P38/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_RocketBody.Succeeded())
	{
		RocketBody->SetStaticMesh(SM_RocketBody.Object);
	}

	//static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_RocketMaterial(TEXT("MaterialInstanceConstant'/Game/New_P38/Materials/MI_P38_Rocket.MI_P38_Rocket'"));
	//if (MI_RocketMaterial.Succeeded())
	//{
	//	RocketBody->SetMaterial(0, MI_RocketMaterial.Object);
	//}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.f;
	Movement->MaxSpeed = 6000.f;
	Movement->ProjectileGravityScale = 0.f;
	

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (P_Explosion.Succeeded())
	{
		Particle = P_Explosion.Object;
	}

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

void AMyRocket::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this) && (OtherActor) != Cast<AMyPawn>(OtherActor) && OtherComp)
	{
		if (Particle)
		{
			if (OtherActor -> ActorHasTag("Enemy"))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorTransform());
			}
		}
		Destroy();
	}
}

