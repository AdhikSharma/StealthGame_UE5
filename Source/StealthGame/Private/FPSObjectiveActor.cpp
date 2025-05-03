// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ShpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	ShpereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShpereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShpereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	ShpereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this,PickupFX,GetActorLocation());
}

// Called every frame
void AFPSObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//this is kinda like cathall as we have added collision on sphere comp
void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);
	PlayEffects();
}

