// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "StealthGame/StealthGameCharacter.h"
#include "StealthGame/StealthGameGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExtractionZone::AExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.f));
	OverlapComp->SetHiddenInGame(false);
	RootComponent = OverlapComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.f, 200.f, 200.f);
	DecalComp->SetupAttachment(OverlapComp);
}

// Called when the game starts or when spawned
void AExtractionZone::BeginPlay()
{
	Super::BeginPlay();
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this,&AExtractionZone::HandleOverlap);
}

void AExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AStealthGameCharacter* character = Cast<AStealthGameCharacter>(OtherActor);

	if (character) 
	{
		if (character->IsCarryingObjective)
		{
			AStealthGameGameMode* gameMode = Cast<AStealthGameGameMode>(GetWorld()->GetAuthGameMode());

			if (gameMode)
			{
				gameMode->MissionComplete(character);
				UGameplayStatics::PlaySoundAtLocation(this, ObjectiveMissionSound, GetActorLocation());
			}
		}
		else 
		{
			UGameplayStatics::PlaySoundAtLocation(this, ObjectiveFailedMissionSound, GetActorLocation());
		}
	}

}


