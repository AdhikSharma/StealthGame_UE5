// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "Components/BoxComponent.h"
#include "StealthGame/StealthGameCharacter.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Overlap Comp"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OverlapBoxOverlap);
}

void ALaunchPad::OverlapBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//try cast to character
	UE_LOG(LogTemp, Warning, TEXT("%s"),*OtherActor->GetName());

	AStealthGameCharacter* character = Cast<AStealthGameCharacter>(OtherActor);

	if (character) 
	{
		FVector LaunchDirection = GetActorForwardVector();
		FVector LaunchVelocity = LaunchDirection * ForwardForce;
		LaunchVelocity.Z += UpForce;
		character->LaunchCharacter(LaunchVelocity,true, true);
		return;
	}

	UPrimitiveComponent* primiticeComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
	if (primiticeComp && primiticeComp->IsSimulatingPhysics())
	{
		FVector LaunchDirection = GetActorForwardVector();
		FVector LaunchVelocity = LaunchDirection * ForwardForce;
		LaunchVelocity.Z += UpForce;

		primiticeComp->AddImpulse(LaunchVelocity, NAME_None, true);
	}


}


