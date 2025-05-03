// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerShpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerShpereComp->SetSphereRadius(300);
	InnerShpereComp->SetupAttachment(MeshComp);

	InnerShpereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);

	OuterShpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OuterShpereComp"));
	OuterShpereComp->SetSphereRadius(3000);
	OuterShpereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappingActors;
	OuterShpereComp->GetOverlappingActors(OverlappingActors);

	for (int32 i=0;i< OverlappingActors.Num();i++)
	{
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(OverlappingActors[i]->GetRootComponent());
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			FVector Direction = GetActorLocation() - OverlappingActors[i]->GetActorLocation();
			Direction.Normalize();

			const float ForceStrength = -2000.f;
			const float sphereRadius = OuterShpereComp->GetScaledSphereRadius();
			PrimComp->AddRadialForce(GetActorLocation(), sphereRadius, ForceStrength,ERadialImpulseFalloff::RIF_Constant,true);
		}
	}

}

void ABlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"),*OtherActor->GetName());
	if (OtherActor) 
	{
		OtherActor->Destroy();
	}
}

