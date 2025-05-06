// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuard.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAIGuard::AAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightHearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("SightHearingConfig"));

	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = 2500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	SightHearingConfig->HearingRange = 2000.f;
	SightHearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightHearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);

	AIPerceptionComponent->ConfigureSense(*SightHearingConfig);
}

// Called when the game starts or when spawned
void AAIGuard::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIGuard::OnPawnSeen);
	
}

void AAIGuard::OnPawnSeen(AActor* actor, FAIStimulus stimulus)
{
	if (actor == nullptr) return;

	if (stimulus.WasSuccessfullySensed())
	{

		if (stimulus.Type == SightConfig->GetSenseID())
		{
			DrawDebugSphere(GetWorld(), actor->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);
		}
		else if (stimulus.Type == SightHearingConfig->GetSenseID())
		{
			DrawDebugSphere(GetWorld(), stimulus.StimulusLocation, 32.f, 12, FColor::Black, false, 10.f);
		}
		else
		{
			
		}
	}
}

// Called every frame
void AAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


